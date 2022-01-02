#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glad/glad.h>
#include <stdlib.h>
#include <vector>

#include "shader.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"

using namespace std;
using namespace glm;

class Mesh {
protected:
	VBO* vbo = NULL;
	EBO* ebo = NULL;
	vector<Texture> tx;
	vector<Texture> spec;

	size_t iSize;
public:
	Mesh() {}

	Mesh(vector<GLfloat>, vector<GLuint>, vector<Texture>);

	void setTexture(const char*);
	void setSpecular(const char*);

	void registerTextures(Shader&);

	void bind();
	void unbind();

	void bindTextures();
	void unbindTextures();

	void destroy();

	inline VBO* getVBO() {
		return vbo;
	}

	inline EBO* getEBO() {
		return ebo;
	}

	inline size_t getISize() {
		return iSize / sizeof(int);
	}

	~Mesh() {
		//destroy();
	}
};


class ObjectRectangular : public Mesh {
public:
	ObjectRectangular();

	ObjectRectangular(const char* image, const char* specular) : ObjectRectangular() {
		setTexture(image);

		if (specular != NULL)
			setSpecular(specular);
	}
};

class ObjectCube: public Mesh {
public:
	ObjectCube();

	ObjectCube(const char* image, const char* specular) : ObjectCube() {
		setTexture(image);

		if (specular != NULL)
			setSpecular(specular);
	}
};

class ObjectPyramid : public Mesh {
public:
	ObjectPyramid();

	ObjectPyramid(const char* image, const char* specular) : ObjectPyramid() {
		setTexture(image);

		if (specular != NULL)
			setSpecular(specular);
	}
};

class ObjectFlat : public Mesh {
public:
	ObjectFlat();

	ObjectFlat(const char* image, const char* specular) : ObjectFlat() {
		setTexture(image);

		if (specular != NULL)
			setSpecular(specular);
	}
};

class ObjectBlank : public Mesh {
public:
	ObjectBlank();
};

class ObjectEmpty : public Mesh {
public:
	ObjectEmpty();
};

#endif