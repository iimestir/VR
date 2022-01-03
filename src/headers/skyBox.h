#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/camera.h"
#include "shader.h"

using namespace std;
using namespace glm;

class SkyBox {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint txID;

	Shader sbShader;

public:
	SkyBox(const char*, const char*, vector<string>);

	void activateShader();

	void draw(Camera*, unsigned, unsigned);
};

#endif