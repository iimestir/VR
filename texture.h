#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shader.h"

class Texture
{
private:
	GLuint ID;
	GLenum type;
public:
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void bind();
	void unbind();
	void deleteTexture();

	inline GLuint getID() {
		return Texture::ID;
	}

	inline GLenum getType() {
		return Texture::type;
	}
};
#endif