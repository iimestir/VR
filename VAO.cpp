#include "VAO.h"

VAO::VAO(bool bind) {
	glGenVertexArrays(1, &ID);

	if (bind)
		VAO::bind();
}

void VAO::linkAttributes(VBO& VBO, GLuint layout, GLuint nComp, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.bind();

	// the stride is the distance in bytes between the beginning and the end of a vertex
	// offset is the offset to get the color values in the vertex (offsetting the coordinates values)

	glVertexAttribPointer(layout, nComp, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	VBO.unbind();
}

void VAO::linkObject3D(Object3D obj) {
	linkAttributes(*obj.getVBO(), 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	linkAttributes(*obj.getVBO(), 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	linkAttributes(*obj.getVBO(), 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

void VAO::bind() {
	glBindVertexArray(ID);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

void VAO::deleteAO() {
	glDeleteVertexArrays(1, &ID);
}
