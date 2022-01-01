#ifndef FBO_H
#define FBO_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "VAO.h"

using namespace std;

enum PPType {
	DEFAULT = 0,
	INVERT = 1,
	BW = 2,
	BORDER = 3,
	OUTLINE = 4
};

class PostProcess {
private:
	GLuint fbo;
	GLuint rbo;
	GLuint txID;

	Shader ppShader;
	PPType type;

	unsigned int frameVAO;
	unsigned int frameVBO;
public:
	PostProcess(const char*, const char*, unsigned, unsigned);

	void bindFBO();
	void unbindFBO();

	void draw();

	void setFrameTextureUni(GLuint);
	int getUniformLocation(const char*);

	void activateShader();

	void setPPType(PPType type);

	inline PPType getPPType() {
		return type;
	}

	inline GLuint getFBOID() {
		return fbo;
	}

	inline GLuint getRBOID() {
		return rbo;
	}

	inline GLuint getTxID() {
		return txID;
	}
};

#endif // FBO_H
