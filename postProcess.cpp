#include "postProcess.h"

PostProcess::PostProcess(const char* vFile, const char* fFile, unsigned width, unsigned height)
	: ppShader(Shader(vFile, fFile)), type(PPType::DEFAULT) {
	activateShader();

	// Setup framebuffer's VAO and VBO
	float rectangleVertices[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &frameVAO);
	glGenBuffers(1, &frameVBO);
	glBindVertexArray(frameVAO);
	glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	// Setup framebuffer FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// FBO texture
	glGenTextures(1, &txID);
	glBindTexture(GL_TEXTURE_2D, txID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, txID, 0);

	// Set up render buffer RBO
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// unbinds all so we make sure we won't touch it accidentally
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcess::bindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PostProcess::unbindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::draw() {
	activateShader();

	glBindVertexArray(frameVAO);

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, txID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
}

void PostProcess::setFrameTextureUni(GLuint uni) {
	activateShader();
	glUniform1i(getUniformLocation("screenTexture"), uni);
}

int PostProcess::getUniformLocation(const char* location) {
	return ppShader.getUniformLocation(location);
}

void PostProcess::activateShader() {
	ppShader.activateShader();
}

void PostProcess::setPPType(PPType type) {
	this->type = type;
}
