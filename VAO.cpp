#include "VAO.h"

VAO::VAO(bool bind) {
	glGenVertexArrays(1, &ID);

	if (bind)
		VAO::bind();
}

void VAO::addMesh(Mesh obj) {
	linkMesh(obj);
	this->objects.push_back(obj);
}

void VAO::linkAttributes(VBO& VBO, GLuint layout, GLuint nComp, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.bind();

	// the stride is the distance in bytes between the beginning and the end of a vertex
	// offset is the offset to get the color values in the vertex (offsetting the coordinates values)

	glVertexAttribPointer(layout, nComp, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	VBO.unbind();
}

void VAO::linkMesh(Mesh obj) {
	linkAttributes(*obj.getVBO(), 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	linkAttributes(*obj.getVBO(), 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	linkAttributes(*obj.getVBO(), 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	linkAttributes(*obj.getVBO(), 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
}

void VAO::bind() {
	glBindVertexArray(ID);

	for (Mesh obj : objects)
		obj.bind();
}

void VAO::draw() {
	for (Mesh obj : objects)
		glDrawElements(GL_TRIANGLES, obj.getISize(), GL_UNSIGNED_INT, 0);
}

void VAO::unbind() {
	for (Mesh obj : objects)
		obj.unbind();

	glBindVertexArray(0);
}

void VAO::destroy() {
	for (Mesh obj : objects)
		obj.destroy();

	glDeleteVertexArrays(1, &ID);
}

void VAO::setAlpha(float alpha) {
	this->alpha = alpha;
}

void VAO::bindMeshTextures() {
	for (Mesh obj : objects)
		obj.bindTextures();
}

void VAO::unbindMeshTextures() {
	for (Mesh obj : objects)
		obj.unbindTextures();
}

void VAO::registerMeshTextures(Shader& shader) {
	for (Mesh mesh : objects)
		mesh.registerTextures(shader);
}