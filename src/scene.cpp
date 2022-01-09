#include "headers/scene.h"

void Scene::registerVertexOnShader(Shader& shader, unsigned vertexIndex) {
	mat4 model = vertices.at(vertexIndex).getMatrix();

	shader.activateShader();

	glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, value_ptr(model));
}

void Scene::registerLightOnShader(unsigned lightIndex) {
	mat4 model = lights.at(lightIndex).getVAO().getMatrix();

	lights.at(lightIndex).getShader().activateShader();

	glUniformMatrix4fv(lights.at(lightIndex).getShader().getUniformLocation("model"), 1, GL_FALSE, value_ptr(model));
}

void Scene::updateLightsUni() {
	activateShader();

	vector<GLfloat> pos;
	vector<GLfloat> colors;
	vector<GLfloat> types;
	vector<GLfloat> orientations;

	for (int i = 0; i < lights.size(); i++) {
		pos.push_back(lights.at(i).getVAO().getTranslation().r);
		pos.push_back(lights.at(i).getVAO().getTranslation().g);
		pos.push_back(lights.at(i).getVAO().getTranslation().b);

		colors.push_back(lights.at(i).getR());
		colors.push_back(lights.at(i).getG());
		colors.push_back(lights.at(i).getB());

		types.push_back(float(lights.at(i).getType()));

		if (lights.at(i).hasOrientationBounds()) {
			vector<DFloat> oriBinds = lights.at(i).getOrientationBounds();
			orientations.push_back(oriBinds.at(0).value());
			orientations.push_back(oriBinds.at(1).value());
			orientations.push_back(oriBinds.at(2).value());
		} else {
			vec3 ori = lights.at(i).getOrientation();

			orientations.push_back(ori.x);
			orientations.push_back(ori.y);
			orientations.push_back(ori.z);
		}
	}

	glUniform1i(program.getUniformLocation("lightSize"), lights.size());
	glUniform1fv(program.getUniformLocation("lightPos"), pos.size(), pos.data());
	glUniform1fv(program.getUniformLocation("lightColor"), colors.size(), colors.data());
	glUniform1fv(program.getUniformLocation("lightType"), types.size(), types.data());
	glUniform1fv(program.getUniformLocation("lightOrientation"), orientations.size(), orientations.data());
}

void Scene::resetLightsUni() {
	activateShader();

	vector<GLfloat> pos;
	vector<GLfloat> colors;
	vector<GLfloat> types;
	vector<GLfloat> orientations;

	pos.push_back(1.0f);
	pos.push_back(1.0f);
	pos.push_back(1.0f);

	colors.push_back(1.0f);
	colors.push_back(1.0f);
	colors.push_back(1.0f);

	orientations.push_back(0.0f);
	orientations.push_back(0.0f);
	orientations.push_back(0.0f);

	types.push_back(float(LightType::DIRECT));

	glUniform1i(program.getUniformLocation("lightSize"), lights.size());
	glUniform1fv(program.getUniformLocation("lightPos"), pos.size(), pos.data());
	glUniform1fv(program.getUniformLocation("lightColor"), colors.size(), colors.data());
	glUniform1fv(program.getUniformLocation("lightType"), types.size(), types.data());
	glUniform1fv(program.getUniformLocation("lightOrientation"), orientations.size(), orientations.data());
}

void Scene::translateOnShader(Shader& shader, unsigned vertexIndex, float xd, float yd, float zd, bool light) {
	VAO* vertice;

	if (light)
		vertice = &lights.at(vertexIndex).getVAO();
	else
		vertice = &vertices.at(vertexIndex);

	vertice->translate(xd, yd, zd);
	vec3 position = vertice->getTranslation();

	mat4 translation = mat4(1.0f);
	translation = translate(translation, position);

	shader.activateShader();

	glUniformMatrix4fv(shader.getUniformLocation("translation"), 1, GL_FALSE, value_ptr(translation));
}

void Scene::rotateOnShader(Shader& shader, unsigned vertexIndex, float wd, float xd, float yd, float zd) {
	VAO* vertice = &vertices.at(vertexIndex);

	vertice->rotate(wd, xd, yd, zd);
	quat position = vertice->getRotation();

	mat4 rotation = mat4(1.0f);
	rotation = mat4_cast(position);

	shader.activateShader();

	glUniformMatrix4fv(shader.getUniformLocation("rotation"), 1, GL_FALSE, value_ptr(rotation));
}

void Scene::scaleOnShader(Shader& shader, unsigned vertexIndex, float xd, float yd, float zd) {
	vertices.at(vertexIndex).scale(xd, yd, zd);
	vec3 position = vertices.at(vertexIndex).getScale();

	mat4 scaling = mat4(1.0f);
	scaling = scale(scaling, position);

	shader.activateShader();

	glUniformMatrix4fv(shader.getUniformLocation("scale"), 1, GL_FALSE, value_ptr(scaling));
}

void Scene::alphaOnShader(Shader& shader, float alpha) {
	shader.activateShader();
	glUniform1i(shader.getUniformLocation("vAlpha"), alpha);
}

void Scene::setDepthUniform(Shader& shader) {
	shader.activateShader();
	glUniform4fv(shader.getUniformLocation("depthColor"), 1, value_ptr(depthColor));
}

void Scene::notifyCameraPosition(Camera* camera) {
	activateShader();

	glUniform3f(program.getUniformLocation("camPos"),
		camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
}


// CONSTRUCTOR
Scene::Scene(const char* vFile, const char* fFile, unsigned width, unsigned height, vector<string> skyFaces)
	: program(Shader(vFile, fFile)),
	  pp(PostProcess("shaders/framebuffer.vs", "shaders/framebuffer.fs", width, height)),
	  sb(SkyBox("shaders/sky.vs", "shaders/sky.fs", skyFaces)) {

	pp.activateShader();
	pp.setFrameTextureUni(0);

	vec2 dim = vec2(float(width), float(height));
	glUniform2fv(pp.getUniformLocation("resolution"), 1, value_ptr(dim));

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Culling
	enableCulling();
}

// RENDER FUNCTION
void Scene::render(GLFWwindow* window, Camera* camera, unsigned width, unsigned height) {
	// PostProcess
	pp.activateShader();
	glUniform1i(pp.getUniformLocation("ppType"), pp.getPPType());
	pp.bindFBO();

	setGLColor(depthColor.r, depthColor.g, depthColor.b, depthColor.a);

	// Camera setup
	//camera->setColliders(getColliders());
	camera->defineInputs(window);
	camera->updateMatrix(0.1f, 100.0f);

	setCameraMatrix(camera);

	// Draw vertices
	draw(camera, width, height);

	// PostProcess ending
	pp.unbindFBO();
	pp.draw();

	glfwSwapBuffers(window);						// Swap the back and front buffer
	glfwPollEvents();								// Takes care of all GLFW events
}

void Scene::activateShader() {
	program.activateShader();
}

void Scene::activateLightShader(unsigned index) {
	lights.at(index).getShader().activateShader();
}

void Scene::destroy() {
	for (VAO vertex : vertices)
		vertex.destroy();

	program.deleteShader();
}

// DRAW FUNCTION
void Scene::draw(Camera* camera, unsigned width, unsigned height) {
	program.activateShader();

	updateLightsUni();
	setDepthUniform(program);

	// enables transparency for alpha < 1.0f
	glEnable(GL_BLEND);

	drawVertices(camera, width, height);
	drawLights(camera, width, height);

	glDisable(GL_BLEND);

	drawUnLightedObjects(camera, width, height);
}

void Scene::drawVertices(Camera* camera, unsigned width, unsigned height) {
	program.activateShader();

	// VERTICES
	for (int i = 0; i < vertices.size(); i++) {
		vertices.at(i).bind();

		mat4 matrix = vertices.at(i).getMatrix();
		quat rotation = vertices.at(i).getRotation();
		vec3 scaling = vertices.at(i).getScale();

		bool oriBound = vertices.at(i).hasOrientationBounds();
		bool posiBound = vertices.at(i).hasPositionBounds();

		// Vertex Position
		if (posiBound) {
			vector<DFloat> binds = vertices.at(i).getPositionBounds();
			translateOnShader(program, i, binds.at(0).value(), binds.at(1).value(), binds.at(2).value());
		}
		else {
			vec3 translation = vertices.at(i).getTranslation();
			translateOnShader(program, i, translation.r, translation.g, translation.b);
		}

		// Vertex Rotation-Orientation
		if (oriBound) {
			vector<DFloat> oriBinds = vertices.at(i).getOrientationBounds();

			float x = oriBinds.at(0).value();
			float y = oriBinds.at(1).value();
			float z = oriBinds.at(2).value();

			// TODO : rotation bound not working as expected
			//mat4 look = lookAt(vec3(0.0f, 0.0f, 0.0f), (vec3(0.0f, 0.0f, 0.0f) + vec3(radians(z), -radians(y), radians(x))), vec3(0.0f, 1.0f, 0.0f));
			//glUniformMatrix4fv(program.getUniformLocation("rotation"), 1, GL_FALSE, value_ptr(look));

			rotateOnShader(program, i, 0.0f, x, y, z);
		}
		else {
			quat rotation = vertices.at(i).getRotation();
			rotateOnShader(program, i, rotation.w, rotation.x, rotation.y, rotation.z);
		}

		scaleVertex(i, scaling.r, scaling.g, scaling.b);
		//alphaOnShader(program, 1.0f);

		registerVertexOnShader(program, i);

		vertices.at(i).registerMeshTextures(program);
		vertices.at(i).bindMeshTextures();
		vertices.at(i).draw();
	}
}

void Scene::drawLights(Camera* camera, unsigned width, unsigned height) {
	for (int i = 0; i < lights.size(); i++) {
		vec3 translation = lights.at(i).getVAO().getTranslation();

		lights.at(i).getShader().activateShader();
		lights.at(i).getVAO().bind();

		// Light position
		if (lights.at(i).getVAO().hasPositionBounds()) {
			vector<DFloat> binds = lights.at(i).getVAO().getPositionBounds();
			translateOnShader(lights.at(i).getShader(), i, binds.at(0).value(), binds.at(1).value(), binds.at(2).value(), true);
		}
		else
			translateOnShader(lights.at(i).getShader(), i, translation.r, translation.g, translation.b, true);

		registerLightOnShader(i);

		setDepthUniform(lights.at(i).getShader());

		lights.at(i).getVAO().draw();
	}
}

void Scene::drawUnLightedObjects(Camera* camera, unsigned width, unsigned height) {
	resetLightsUni();

	// SkyBox
	sb.draw(camera, width, height);
}

void Scene::setGLColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	// "Adds" a color to the back buffer
	glClearColor(red, green, blue, alpha);
	// Clears what's in the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::setBackgroundColor(GLFWwindow* window, unsigned int width, unsigned int height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	depthColor = vec4(red, green, blue, alpha);
	setGLColor(red, green, blue, alpha);
	glfwSwapBuffers(window);
}

void Scene::setPPType(PPType type) {
	pp.setPPType(type);
}

unsigned Scene::addMesh(Mesh obj, float posX, float posY, float posZ, float alpha) {
	VAO vao(true);
	obj.bind();

	vao.addMesh(obj);
	vao.translate(posX, posY, posZ);

	vao.unbind();
	obj.unbind();

	vao.setAlpha(alpha);
	vertices.push_back(vao);

	unsigned index = vertices.size() - 1;

	return index;
}

vector<Texture> Scene::retrieveMeshTextures(const aiScene* pScene, aiMesh* aiMesh, const char* path) {
	vector<Texture> textures;

	// Checks if the material is already loaded
	auto it = loadedMaterials.find(aiMesh->mMaterialIndex);
	if (it != loadedMaterials.end())
		return it->second;

	const aiMaterial* material = pScene->mMaterials[aiMesh->mMaterialIndex];

	aiString aiPath;

	string fileStr = string(path);
	string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			cout << "Loading diffuse : " << aiPath.data;
			textures.push_back(Texture((fileDirectory + aiPath.data).c_str(), "tex0", 0));
			cout << " OK" << endl;
		}
	}

	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		if (material->GetTexture(aiTextureType_SPECULAR, 0, &aiPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			cout << "Loading specular : " << aiPath.data;
			textures.push_back(Texture((fileDirectory + aiPath.data).c_str(), "tex1", 1));
			cout << " OK" << endl;
		}
	}

	loadedMaterials[aiMesh->mMaterialIndex] = textures;

	return textures;
}

Mesh Scene::retrieveMesh(const aiScene* pScene, aiMesh* aiMesh, const char* path, bool col) {
	vector<GLfloat> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	// Vertices
	for (unsigned i = 0; i < aiMesh->mNumVertices; i++) {
		// Coordinates
		vertices.push_back(aiMesh->mVertices[i].z);
		vertices.push_back(aiMesh->mVertices[i].y);
		vertices.push_back(aiMesh->mVertices[i].x);

		// Colors
		if (aiMesh->HasVertexColors(0)) {
			vertices.push_back(aiMesh->mColors[0][i].r);
			vertices.push_back(aiMesh->mColors[0][i].g);
			vertices.push_back(aiMesh->mColors[0][i].b);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		// Textures Coords
		if (aiMesh->HasTextureCoords(0)) {
			vertices.push_back(aiMesh->mTextureCoords[0][i].y);
			vertices.push_back(aiMesh->mTextureCoords[0][i].x);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		// Normals
		vertices.push_back(aiMesh->mNormals[i].x);
		vertices.push_back(aiMesh->mNormals[i].y);
		vertices.push_back(aiMesh->mNormals[i].z);
	}

	// Indices
	for (unsigned i = 0; i < aiMesh->mNumFaces; i++) {
		const aiFace& face = aiMesh->mFaces[i];

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	for(Texture tx : retrieveMeshTextures(pScene, aiMesh, path))
		textures.push_back(tx);

	return Mesh(vertices, indices, textures, col);
}

vector<unsigned> Scene::loadMesh(const char* path, bool col) {
	vector<unsigned> ids;

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!pScene) {
		cout << "Error while parsing" << endl;
		return ids;
	}

	for (int i = 0; i < pScene->mNumMeshes; i++)
		ids.push_back(addMesh(retrieveMesh(pScene, pScene->mMeshes[i], path, col)));

	return ids;
}

unsigned Scene::addLight(Light light) {
	lights.push_back(light);

	unsigned index = lights.size() - 1;

	setLightColor(index, light.getR(), light.getG(), light.getB(), light.getAlpha());

	return index;
}

void Scene::bindVertexPosition(unsigned vertexIndex, DFloat posX, DFloat posY, DFloat posZ) {
	VAO* vertex = &vertices.at(vertexIndex);

	vertex->bindPositionTo(posX, posY, posZ);
}

void Scene::bindVertexOrientation(unsigned vertexIndex, DFloat rotX, DFloat rotY, DFloat rotZ) {
	VAO* vertex = &vertices.at(vertexIndex);

	vertex->bindRotationTo(rotX, rotY, rotZ);
}

void Scene::translateVertex(unsigned vertexIndex, float xd, float yd, float zd) {
	translateOnShader(program, vertexIndex, xd, yd, zd);
}

void Scene::rotateVertex(unsigned vertexIndex, float xd, float yd, float zd) {
	vec3 euler(xd, yd, zd);
	quat quaterion = quat(euler);

	rotateOnShader(program, vertexIndex, quaterion.w, quaterion.x, quaterion.y, quaterion.z);
}

void Scene::scaleVertex(unsigned vertexIndex, float xd, float yd, float zd) {
	scaleOnShader(program, vertexIndex, xd, yd, zd);
}

void Scene::setLightColor(unsigned index, float red, float green, float blue, float alpha) {
	getLightShader(index)->activateShader();

	glUniform4f(getLightShader(index)->getUniformLocation("lightColor"), red, green, blue, alpha);
}

void Scene::setCameraMatrix(Camera* camera) {
	notifyCameraPosition(camera);

	activateShader();
	camera->sendMatrixToShader(program);

	for (Light light : lights) {
		light.getShader().activateShader();
		camera->sendMatrixToShader(light.getShader());
	}
}

vector<Colliders> Scene::getColliders() {
	vector<Colliders> colliders;
	for (VAO vao : vertices) {
		vector<Colliders> vaoC = vao.getColliders();
		colliders.insert(colliders.end(), vaoC.begin(), vaoC.end());
	}

	return colliders;
}

void Scene::destroyVertex(unsigned index) {
	vertices.at(index).destroy();
	vertices.erase(vertices.begin() + index);
}

bool Scene::collidesWith(float x, float y, float z) {
	for (VAO vao : vertices)
		if (vao.collidesWith(x, y, z)) return true;

	return false;
}

bool Scene::collidesWith(vec3 pos) {
	return collidesWith(pos.r, pos.g, pos.b);
}

void Scene::enableCulling() {
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
}

void Scene::disableCulling() {
	glDisable(GL_CULL_FACE);
}
