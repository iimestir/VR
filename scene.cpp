#include "scene.h"

void Scene::registerOnShader(Shader& shader, unsigned vertexIndex) {
	mat4 model = vertices.at(vertexIndex).getMatrix();

	shader.activateShader();

	glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, value_ptr(model));
}

void Scene::colorShader(Shader& shader, float red, float green, float blue, float alpha) {
	shader.activateShader();
	glUniform4f(shader.getUniformLocation("lightColor"), red, green, blue, alpha);
}

void Scene::setLightPosition(float x, float y, float z) {
	activateShader();

	// TODO : gérer plusieures lumières
	glUniform3f(program.getUniformLocation("lightPos"), x, y, z);
}

void Scene::translateOnShader(Shader& shader, unsigned vertexIndex, float xd, float yd, float zd, bool light) {
	VAO* vertice;
	string uniform;

	if (light)
		vertice = &lights.at(vertexIndex).second;
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

void Scene::notifyCameraPosition(Camera* camera) {
	activateShader();

	glUniform3f(program.getUniformLocation("camPos"),
		camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
}

Scene::Scene(const char* vFile, const char* fFile) : program(Shader(vFile, fFile)) {
	setShaderColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Scene::render(GLFWwindow* window, Camera* camera) {
	setGLColor(0.07f, 0.13f, 0.17f);

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
	lights.at(index).first.activateShader();
}

void Scene::destroy() {
	for (VAO vertex : vertices)
		vertex.destroy();

	program.deleteShader();
}

void Scene::draw() {
	activateShader();

	for (int i = 0; i < vertices.size(); i++) {
		vertices.at(i).bind();

		vertices.at(i).registerMeshTextures(program);
		
		mat4 matrix = vertices.at(i).getMatrix();
		vec3 translation = vertices.at(i).getTranslation();
		quat rotation = vertices.at(i).getRotation();
		vec3 scaling = vertices.at(i).getScale();

		translateOnGlobal(i, translation.r, translation.g, translation.b);
		rotateOnGlobal(i, rotation.w, rotation.x, rotation.y, rotation.z);
		scaleOnGlobal(i, scaling.r, scaling.g, scaling.b);
		registerOnShader(program, i);

		vertices.at(i).bindMeshTextures();
		vertices.at(i).draw();
	}

	for (int i = 0; i < lights.size(); i++) {
		vec3 translation = lights.at(i).second.getTranslation();
		setLightPosition(translation.r, translation.g, translation.b);

		lights.at(i).first.activateShader();

		translateOnShader(lights.at(i).first, i, translation.r, translation.g, translation.b, true);
		registerOnShader(lights.at(i).first, i);

		lights.at(i).second.bind();
		lights.at(i).second.draw();
	}
}

void Scene::refreshTextures() {
	for (VAO vertex : vertices)
		vertex.bindMeshTextures();
}

void Scene::setGLColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	// "Adds" a color to the back buffer
	glClearColor(red, green, blue, alpha);
	// Clears what's in the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::setBackgroundColor(GLFWwindow* window, unsigned int width, unsigned int height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	// Creates the work zone
	glViewport(0, 0, width, height);
	setGLColor(red, green, blue, alpha);

	// Swaps the front and back buffers
	glfwSwapBuffers(window);
}

unsigned Scene::addMesh(Mesh obj, float posX, float posY, float posZ) {
	activateShader();

	VAO vao(true);
	obj.bind();

	vao.addMesh(obj);

	vao.unbind();
	obj.unbind();

	vertices.push_back(vao);

	unsigned index = vertices.size() - 1;
	translateObject(index, posX, posY, posZ);

	return index;
}

unsigned Scene::addLight(Mesh obj, const char* vFile, const char* fFile, float posX, float posY, float posZ, float red, float green, float blue, float alpha) {
	VAO vao(true);
	vao.addMesh(obj);
	vao.unbind();

	lights.push_back(std::make_pair(Shader(vFile, fFile),vao));

	unsigned index = lights.size() - 1;

	translateLight(index, posX, posY, posZ);
	setLightShaderColor(index, red, green, blue, alpha);

	return index;
}

void Scene::translateOnGlobal(unsigned vertexIndex, float xd, float yd, float zd) {
	translateOnShader(program, vertexIndex, xd, yd, zd);
}

void Scene::rotateOnGlobal(unsigned vertexIndex, float wd, float xd, float yd, float zd) {
	rotateOnShader(program, vertexIndex, wd, xd, yd, zd);
}

void Scene::scaleOnGlobal(unsigned vertexIndex, float xd, float yd, float zd) {
	scaleOnShader(program, vertexIndex, xd, yd, zd);
}

void Scene::translateObject(unsigned index, float xd, float yd, float zd) {
	vertices.at(index).translate(xd, yd, zd);
}

void Scene::translateLight(unsigned index, float xd, float yd, float zd) {
	lights.at(index).second.translate(xd, yd, zd);
}

void Scene::setLightShaderColor(unsigned index, float red, float green, float blue, float alpha) {
	colorShader(*getLightShader(index), red, green, blue, alpha);

	// TODO : gérer plusieures lumières
	setShaderColor(red, green, blue, alpha);
}

void Scene::setShaderColor(float red, float green, float blue, float alpha) {
	colorShader(program, red, green, blue, alpha);
}

void Scene::setCameraMatrix(Camera* camera) {
	notifyCameraPosition(camera);

	activateShader();
	camera->setupMatrix(program);

	for (pair<Shader, VAO> light : lights) {
		light.first.activateShader();
		camera->setupMatrix(light.first);
	}
}
