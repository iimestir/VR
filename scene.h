#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

using namespace std;
using namespace glm;

class Scene {
private:
	Shader program;
	vector<VAO> vertexes;
	vector<pair<Shader, VAO>> lights;

	void translateGlobal(Shader*, unsigned, float, float, float);
	void colorShader(Shader*, float, float, float, float);
	void setLightPosition(float, float, float);

	void notifyCameraPosition(Camera*);
public:
	Scene(const char*, const char*);

	void activateShader();
	void activateLightShader(unsigned);

	void destroy();

	void draw();
	void refreshTextures();

	void setGLColor(GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);
	void setBackgroundColor(GLFWwindow*, unsigned int, unsigned int, GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);

	unsigned addObject3D(Object3D, float = 0.0f, float = 0.0f, float = 0.0f);
	unsigned addLight(Object3D, const char*, const char*, float = 0.0f, float = 0.0f, float = 0.0f);

	void translateObject(unsigned, float, float, float);
	void translateLight(unsigned, float, float, float);
	void setLightShaderColor(unsigned, float, float, float, float);
	void setShaderColor(float, float, float, float);

	void setCameraMatrix(Camera*, const char*);

	inline Shader* getShader() {
		return &program;
	}

	inline Shader* getLightShader(unsigned index) {
		return &lights.at(index).first;
	}

	inline GLuint getShaderID() {
		return program.getID();
	}

	inline VAO getVertex(unsigned index) {
		return vertexes.at(index);
	}
};

#endif
