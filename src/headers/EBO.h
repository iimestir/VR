#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

using namespace std;

class EBO
{
private:
	GLuint ID;

public:
	EBO(GLuint*, GLsizeiptr, bool = true);

	void bind();
	void unbind();
	void deleteBO();

	inline GLuint getID() {
		return EBO::ID;
	}
};

#endif