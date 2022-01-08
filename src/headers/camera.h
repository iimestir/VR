#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <stdio.h>
#include <vector>
#include <math.h>

#include "shader.h"
#include "collider.h"
#include "audio.h"

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 matrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	vector<Colliders> colliders;

	float d = 0.0f;

	bool firstClick = true;
	bool firstStep = true;
	bool freeCam = false;

	int width;
	int height;

	float speed;
	float sensitivity;	
	float fov;
	float initialSpeed;
	float initialSensitivity;

	bool checkCollisions(glm::vec3);
	void move(glm::vec3);
public:
	Camera(int, int, glm::vec3, float = 45.0f, float = 0.1f, float = 100.0f);

	void sendMatrixToShader(Shader&);
	void updateMatrix(float, float);
	void defineInputs(GLFWwindow*);

	void setFOV(float);
	void setColliders(vector<Colliders>);

	inline glm::mat4 getView() {
		return view;
	}

	inline glm::mat4 getProjection() {
		return projection;
	}

	inline glm::vec3 getOrientation() {
		return orientation;
	}

	inline float getFOV() {
		return fov;
	}

	inline float* getPOriX() {
		return &orientation.r;
	}

	inline float* getPOriY() {
		return &orientation.g;
	}

	inline float* getPOriZ() {
		return &orientation.b;
	}

	inline glm::mat4 getMatrix() {
		return matrix;
	}

	inline glm::vec3 getPosition() {
		return position;
	}

	inline float* getPPosX() {
		return &position.r;
	}

	inline float* getPPosY() {
		return &position.g;
	}

	inline float* getPPosZ() {
		return &position.b;
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

	inline glm::vec3 getUP() {
		return up;
	}
};

#endif