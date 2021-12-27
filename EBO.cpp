#include "EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size, bool bind) {
	glGenBuffers(1, &ID);

	if (bind)
		EBO::bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::deleteBO() {
	glDeleteBuffers(1, &ID);
}
