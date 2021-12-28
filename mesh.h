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

	inline void registerTextures(Shader& shader) {
		for(Texture diffusion : tx)
			diffusion.texUnit(shader, "diffuse", 0);

		for (Texture specular : spec)
			specular.texUnit(shader, "specular", 1);
	}

	inline VBO* getVBO() {
		return vbo;
	}

	inline EBO* getEBO() {
		return ebo;
	}

	inline Texture* getTexture(unsigned index) {
		return &tx.at(index);
	}

	inline size_t getISize() {
		return iSize / sizeof(int);
	}

	inline void setTexture(const char* image) {
		tx.push_back(Texture(image, "diffuse", 0));
	}

	inline void setSpecular(const char* specular) {
		spec.push_back(Texture(specular, "specular", 1));
	}

	inline void bind() {
		vbo->bind();
		ebo->bind();
	}

	inline void unbind() {
		vbo->unbind();
		ebo->unbind();
	}

	inline void bindTextures() {
		for (Texture diffusion : tx)
			diffusion.bind();

		for (Texture specular : spec)
			specular.bind();
	}

	inline void unbindTextures() {
		for (Texture diffusion : tx)
			diffusion.unbind();

		for (Texture specular : spec)
			specular.unbind();
	}

	inline void destroy() {
		unbind();

		vbo->deleteBO();
		ebo->deleteBO();

		for (Texture diffusion : tx)
			diffusion.deleteTexture();
		for (Texture specular : spec)
			specular.deleteTexture();

		free(vbo);
		free(ebo);
		tx.clear();
		spec.clear();
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

#endif