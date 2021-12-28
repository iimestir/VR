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
	vector<VAO> vertices;
	vector<pair<Shader, VAO>> lights;

	void registerOnShader(Shader&, unsigned);

	void colorShader(Shader&, float, float, float, float);
	void setLightPosition(float, float, float);

	void translateOnShader(Shader&, unsigned, float, float, float, bool = false);
	void rotateOnShader(Shader&, unsigned, float, float, float, float);
	void scaleOnShader(Shader&, unsigned, float, float, float);

	void notifyCameraPosition(Camera*);
public:
	Scene(const char*, const char*);

	void render(GLFWwindow*, Camera*);

	void activateShader();
	void activateLightShader(unsigned);

	void destroy();

	void draw();
	void refreshTextures();

	void setGLColor(GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);
	void setBackgroundColor(GLFWwindow*, unsigned int, unsigned int, GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);

	unsigned addMesh(Mesh, float = 0.0f, float = 0.0f, float = 0.0f);
	unsigned addLight(Mesh, const char*, const char*, 
		float = 0.0f, float = 0.0f, float = 0.0f, 
		float = 0.0f, float = 0.0f, float = 0.0f, float = 1.0f
	);

	void translateOnGlobal(unsigned, float, float, float);
	void rotateOnGlobal(unsigned, float, float, float, float);
	void scaleOnGlobal(unsigned, float, float, float);

	void translateObject(unsigned, float, float, float);
	void translateLight(unsigned, float, float, float);

	void setLightShaderColor(unsigned, float, float, float, float);
	void setShaderColor(float, float, float, float);

	void setCameraMatrix(Camera*);

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
		return vertices.at(index);
	}
};

#endif
