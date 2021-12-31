#include "scene.h"

void Scene::registerVertexOnShader(unsigned vertexIndex) {
	mat4 model = vertices.at(vertexIndex).getMatrix();

	program.activateShader();

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
	for (int i = 0; i < lights.size(); i++) {
		pos.push_back(lights.at(i).getVAO().getTranslation().r);
		pos.push_back(lights.at(i).getVAO().getTranslation().g);
		pos.push_back(lights.at(i).getVAO().getTranslation().b);
	}

	for (int i = 0; i < lights.size(); i++) {
		colors.push_back(lights.at(i).getR());
		colors.push_back(lights.at(i).getG());
		colors.push_back(lights.at(i).getB());
	}

	for (int i = 0; i < lights.size(); i++) {
		types.push_back(float(lights.at(i).getType()));
	}

	glUniform1i(program.getUniformLocation("lightSize"), lights.size());
	glUniform1fv(program.getUniformLocation("lightPos"), pos.size(), pos.data());
	glUniform1fv(program.getUniformLocation("lightColor"), colors.size(), colors.data());
	glUniform1fv(program.getUniformLocation("lightType"), types.size(), types.data());
}

void Scene::translateOnShader(Shader& shader, unsigned vertexIndex, float xd, float yd, float zd, bool light) {
	VAO* vertice;
	string uniform;

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
	vertices.at(vertexIndex).rotate(wd, xd, yd, zd);
	quat position = vertices.at(vertexIndex).getRotation();

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

Scene::Scene(const char* vFile, const char* fFile) : program(Shader(vFile, fFile)) {}

void Scene::render(GLFWwindow* window, Camera* camera) {
	setGLColor(depthColor.r, depthColor.g, depthColor.b, depthColor.a);

	camera->defineInputs(window);
	camera->updateMatrix(45.0f, 0.1f, 100.0f);

	setCameraMatrix(camera);

	draw();

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

void Scene::draw() {
	activateShader();

	glEnable(GL_BLEND);
	for (int i = 0; i < vertices.size(); i++) {
		vertices.at(i).bind();

		vertices.at(i).registerMeshTextures(program);

		mat4 matrix = vertices.at(i).getMatrix();
		vec3 translation = vertices.at(i).getTranslation();
		quat rotation = vertices.at(i).getRotation();
		vec3 scaling = vertices.at(i).getScale();

		translateVertex(i, translation.r, translation.g, translation.b);
		scaleVertex(i, scaling.r, scaling.g, scaling.b);
		rotateOnShader(program, i, rotation.w, rotation.x, rotation.y, rotation.z);
		//alphaOnShader(program, 1.0f);

		registerVertexOnShader(i);

		vertices.at(i).bindMeshTextures();
		vertices.at(i).draw();
	}

	for (int i = 0; i < lights.size(); i++) {
		vec3 translation = lights.at(i).getVAO().getTranslation();

		lights.at(i).getShader().activateShader();
		lights.at(i).getVAO().bind();

		translateOnShader(lights.at(i).getShader(), i, translation.r, translation.g, translation.b, true);
		registerLightOnShader(i);

		setDepthUniform(lights.at(i).getShader());

		lights.at(i).getVAO().draw();
	}
	glDisable(GL_BLEND);

	updateLightsUni();

	setDepthUniform(program);
}

void Scene::setGLColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	// "Adds" a color to the back buffer
	glClearColor(red, green, blue, alpha);
	// Clears what's in the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::setBackgroundColor(GLFWwindow* window, unsigned int width, unsigned int height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	depthColor = vec4(red, green, blue, alpha);

	// Creates the work zone
	glViewport(0, 0, width, height);
	setGLColor(red, green, blue, alpha);

	// Swaps the front and back buffers
	glfwSwapBuffers(window);
}

unsigned Scene::addMesh(Mesh obj, float posX, float posY, float posZ, float alpha) {
	activateShader();

	VAO vao(true);
	obj.bind();

	vao.addMesh(obj);

	vao.unbind();
	obj.unbind();

	vao.setAlpha(alpha);
	vertices.push_back(vao);

	unsigned index = vertices.size() - 1;
	translateVertex(index, posX, posY, posZ);

	return index;
}

vector<unsigned> Scene::loadMesh(const char* dir) {
	string path = "models/" + string(dir) + "/scene.gltf";

	Model model(path.c_str());

	vector<unsigned> ids;
	for (Mesh mesh : model.getMeshes())
		ids.push_back(addMesh(mesh));

	return ids;
}

unsigned Scene::addLight(Light light) {
	lights.push_back(light);

	unsigned index = lights.size() - 1;

	setLightColor(index, light.getR(), light.getG(), light.getB(), light.getAlpha());

	return index;
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
	camera->setupMatrix(program);

	for (Light light : lights) {
		light.getShader().activateShader();
		camera->setupMatrix(light.getShader());
	}
}
