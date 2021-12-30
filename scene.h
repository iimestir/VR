#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "model.h"
#include "light.h"

using namespace std;
using namespace glm;

class Scene {
private:
	Shader program;
	vector<VAO> vertices;
	vector<Light> lights;

	void registerVertexOnShader(unsigned);
	void registerLightOnShader(unsigned);

	void drawLights();

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

	void setGLColor(GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);
	void setBackgroundColor(GLFWwindow*, unsigned int, unsigned int, GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);

	unsigned addMesh(Mesh, float = 0.0f, float = 0.0f, float = 0.0f);
	vector<unsigned> loadMesh(const char*);

	unsigned addLight(Light);

	void translateVertex(unsigned, float, float, float);
	void rotateVertex(unsigned, float, float, float);
	void scaleVertex(unsigned, float, float, float);

	void setLightColor(unsigned, float, float, float, float);

	void setCameraMatrix(Camera*);

	inline Shader* getShader() {
		return &program;
	}

	inline Shader* getLightShader(unsigned index) {
		return &lights.at(index).getShader();
	}

	inline GLuint getShaderID() {
		return program.getID();
	}

	inline VAO getVertex(unsigned index) {
		return vertices.at(index);
	}
};

#endif
