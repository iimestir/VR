#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>

#include "VBO.h"
#include "object3D.h"

class VAO
{
private:
	GLuint ID;

public:
	VAO(bool = false);

	void linkAttributes(VBO&, GLuint, GLuint, GLenum, GLsizeiptr, void*);

	void linkObject3D(Object3D);

	void bind();
	void unbind();
	void deleteAO();

	inline GLuint getID() {
		return VAO::ID;
	}
};
#endif