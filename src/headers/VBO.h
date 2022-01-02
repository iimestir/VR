#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

using namespace std;

class VBO {
private:
	GLuint ID;

public:
	VBO(GLfloat*, GLsizeiptr, bool = true);

	void bind();
	void unbind();
	void deleteBO();

	inline GLuint getID() {
		return VBO::ID;
	}
};

#endif