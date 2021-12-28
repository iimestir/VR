#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 matrix = glm::mat4(1.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed;
	float sensitivity;	
	float initialSpeed;
	float initialSensitivity;
public:
	Camera(int, int, glm::vec3, float = 0.1f, float = 100.0f);

	void setupMatrix(Shader&);
	void updateMatrix(float, float, float);
	void defineInputs(GLFWwindow*);

	inline glm::vec3 getPosition() {
		return position;
	}

	inline int getWidth() {
		return width;
	}
	
	inline int getHeight() {
		return height;
	}

	inline float getSpeed() {
		return speed;
	}

	inline float getSensitivity() {
		return sensitivity;
	}
};
#endif