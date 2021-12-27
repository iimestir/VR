#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size, bool bind) {
	glGenBuffers(1, &ID);

	if (bind)
		VBO::bind();

	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::deleteBO() {
	glDeleteBuffers(1, &ID);
}