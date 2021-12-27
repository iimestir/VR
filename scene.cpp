#include "scene.h"

void Scene::translateGlobal(Shader* shader, unsigned index, float xd, float yd, float zd) {
	vec3 position = vec3(xd, yd, zd);
	mat4 model = mat4(1.0f);
	model = translate(model, position);

	shader->activateShader();

	glUniformMatrix4fv(shader->getUniformLocation("model"), 1, GL_FALSE, value_ptr(model));
}

void Scene::colorShader(Shader* shader, float red, float green, float blue, float alpha) {
	shader->activateShader();
	glUniform4f(shader->getUniformLocation("lightColor"), red, green, blue, alpha);
}

void Scene::setLightPosition(float x, float y, float z) {
	activateShader();

	glUniform3f(program.getUniformLocation("lightPos"), x, y, z);
}

Scene::Scene(const char* vFile, const char* fFile) : program(Shader(vFile, fFile)) {
	setShaderColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Scene::activateShader() {
	program.activateShader();
}

void Scene::activateLightShader(unsigned index) {
	lights.at(index).first.activateShader();
}

void Scene::destroy() {
	for (VAO vertex : vertexes)
		vertex.destroy();

	program.deleteShader();
}

void Scene::draw() {
	activateShader();

	for (VAO vertex : vertexes) {
		vertex.bindObject3DTextures();
		vertex.bind();
		vertex.draw();
	}

	for (pair<Shader, VAO> light : lights) {
		light.first.activateShader();

		light.second.bind();
		light.second.draw();
	}
}

void Scene::refreshTextures() {
	for (VAO vertex : vertexes)
		vertex.bindObject3DTextures();
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

unsigned Scene::addObject3D(Object3D obj, float posX, float posY, float posZ) {
	VAO vao(true);
	vao.addObject3D(obj);
	vao.unbind();

	vertexes.push_back(vao);

	unsigned index = vertexes.size() - 1;

	translateObject(index, posX, posY, posZ);

	return index;
}

unsigned Scene::addLight(Object3D obj, const char* vFile, const char* fFile, float posX, float posY, float posZ) {
	VAO vao(true);
	vao.addObject3D(obj);
	vao.unbind();

	lights.push_back(std::make_pair(Shader(vFile, fFile),vao));

	unsigned index = lights.size() - 1;

	translateLight(index, posX, posY, posZ);

	return index;
}

void Scene::translateObject(unsigned index, float xd, float yd, float zd) {
	translateGlobal(&program, index, xd, yd, zd);
}

void Scene::translateLight(unsigned index, float xd, float yd, float zd) {
	translateGlobal(&lights.at(index).first, index, xd, yd, zd);
	setLightPosition(xd, yd, zd);
}

void Scene::setLightShaderColor(unsigned index, float red, float green, float blue, float alpha) {
	colorShader(getLightShader(index), red, green, blue, alpha);
}

void Scene::setShaderColor(float red, float green, float blue, float alpha) {
	colorShader(&program, red, green, blue, alpha);
}

void Scene::setCameraMatrix(Camera* camera, const char* location) {
	notifyCameraPosition(camera);

	activateShader();
	camera->setupMatrix(program, location);

	for (pair<Shader, VAO> light : lights) {
		light.first.activateShader();
		camera->setupMatrix(light.first, location);
	}
}

void Scene::notifyCameraPosition(Camera* camera) {
	activateShader();

	glUniform3f(program.getUniformLocation("camPos"), 
		camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
}
