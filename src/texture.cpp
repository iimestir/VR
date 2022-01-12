#include "headers/texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot) {
	type = texType;

	int widthImg, heightImg, numColCh;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);

	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;

	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint t;
	if (type == "tex3")
		t = GL_RED;
	else
		t = GL_RGBA;

	if (numColCh == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, t, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	else if (numColCh == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, t, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	else if (numColCh == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, t, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	unbind();
}

void Texture::registerTexture(Shader& shader, const char* uniform, GLuint unit) {
	bind();
	shader.activateShader();
	glUniform1i(shader.getUniformLocation(uniform), unit);
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture() {
	glDeleteTextures(1, &ID);
}