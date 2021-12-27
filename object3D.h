#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glad/glad.h>
#include <stdlib.h>

#include "shader.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"

class Object3D {
protected:
	VBO* vbo;
	EBO* ebo;
	Texture* tx;

	size_t iSize;
public:
	inline VBO* getVBO() {
		return vbo;
	}

	inline EBO* getEBO() {
		return ebo;
	}

	inline Texture* getTexture() {
		return tx;
	}

	inline size_t getISize() {
		return iSize / sizeof(int);
	}

	inline void setTexture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, Shader& shader, const char* uniform, GLuint unit) {
		tx = new Texture(image, texType, slot, format, pixelType);
		tx->texUnit(shader, uniform, unit);
	}

	inline void bind() {
		vbo->bind();
		ebo->bind();
	}

	inline void unbind() {
		vbo->unbind();
		ebo->unbind();
	}

	inline void bindTexture() {
		if (tx == NULL)
			return;

		tx->bind();
	}

	inline void unbindTexture() {
		tx->unbind();
	}

	inline void destroy() {
		unbind();

		vbo->deleteBO();
		ebo->deleteBO();

		if (tx != NULL)
			tx->deleteTexture();

		free(vbo);
		free(ebo);
		free(tx);
	}

	~Object3D() {
		//destroy();
	}
};


class ObjectRectangular : public Object3D {
public:
	ObjectRectangular();

	ObjectRectangular(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, Shader& shader, const char* uniform, GLuint unit) : ObjectRectangular() {
		setTexture(image, texType, slot, format, pixelType, shader, uniform, unit);
	}
};

class ObjectCube: public Object3D {
public:
	ObjectCube();

	ObjectCube(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, Shader& shader, const char* uniform, GLuint unit) : ObjectCube() {
		setTexture(image, texType, slot, format, pixelType, shader, uniform, unit);
	}
};

class ObjectPyramid : public Object3D {
public:
	ObjectPyramid();

	ObjectPyramid(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, Shader& shader, const char* uniform, GLuint unit) : ObjectPyramid() {
		setTexture(image, texType, slot, format, pixelType, shader, uniform, unit);
	}
};

class ObjectLittleCube: public Object3D {
public:
	ObjectLittleCube();
};

#endif