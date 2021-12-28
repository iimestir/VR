#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shader.h"

class Texture
{
private:
	GLuint ID;
	const char* type;
	GLuint unit;
public:
	Texture(const char* image, const char* texType, GLuint slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void bind();
	void unbind();
	void deleteTexture();

	inline GLuint getID() {
		return Texture::ID;
	}

	inline const char* getType() {
		return Texture::type;
	}
};
#endif