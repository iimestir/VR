#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

using namespace std;

string getFileContents(const char*);


class Shader {
private:
	GLuint ID;

public:
	Shader(const char*, const char*);

	void activateShader();
	void deleteShader();

	void compileErrors(unsigned int, const char*, const char*);

	inline GLuint getID() {
		return Shader::ID;
	}

	inline int getUniformLocation(const char* location) {
		return glGetUniformLocation(ID, location);
	}
};

#endif