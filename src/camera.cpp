#include "headers/camera.h"

bool Camera::checkCollisions(glm::vec3 pos) {
	for (Colliders c : colliders) {
		if (c.collidesWith(pos.r, pos.g, pos.b)) return true;
	}

	return false;
}

Camera::Camera(int width, int height, glm::vec3 position, float fov, float speed, float sensitivity) {
	this->width = width;
	this->height = height;
	this->position = position;
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->initialSpeed = speed;
	this->initialSensitivity = sensitivity;
	this->fov = fov;

	this->initialFov = fov;

	srand(time(NULL));
}

void Camera::sendMatrixToShader(Shader& shader) {
	glUniformMatrix4fv(shader.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Camera::move(glm::vec3 dpos) {
	if (checkCollisions(position + dpos))
		return;

	position += dpos;

	if (this->d > 1.0f) {
		// Step sound
		Audio::getInstance().playSound(footStep, 1);
		footStep = (footStep + 1) % 8;
		this->d = 0.0f;

		// Paranormal sound =)
		if (rand() % 100 <= 1) {
			int audio = rand() % 4 + 8;
			Audio::getInstance().playSound(audio, 2, 70.0f);
		}
	}
}

// Inspired by : https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-
// Victor Godran's "OpenGL Tutorials"
void Camera::defineInputs(GLFWwindow* window) {
	bool stopS = false;

	// Keyboard inputs
	// MOVEMENTS
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move((speed * orientation));

		if (!stopS) {
			this->d += 0.009f * (speed / initialSpeed);
			stopS = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		move((speed * -glm::normalize(glm::cross(orientation, up))));

		if (!stopS) {
			this->d += 0.009f * (speed / initialSpeed);
			stopS = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move((speed * -orientation));

		if (!stopS) {
			this->d += 0.009f * (speed / initialSpeed);
			stopS = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		move((speed * glm::normalize(glm::cross(orientation, up))));

		if (!stopS) {
			this->d += 0.009f * (speed / initialSpeed);
			stopS = true;
		}
	}

	// PRONE & SPRINT
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		speed = 0.3 * initialSpeed;
		position.g = -0.5f;
		fov = initialFov;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 2.0f * initialSpeed;
		fov = 1.07f * initialFov;

		position.g = 0.0f;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
		speed = initialSpeed;
		fov = initialFov;

		position.g = 0.0f;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = initialSpeed;
		fov = initialFov;
	}

	// DEBUG
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
		cout << "World Pos: " << position.r << " " << position.g << " " << position.b << " " << endl;
		cout << "Colliders: " << colliders.size() << endl;
		cout << "d: " << this->d << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		freeCam = !freeCam;
	}

	// Mouse inputs
	if (!freeCam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// hides cursor

		if (firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Fetches the coordinates of the cursor
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Vertical rotation
		glm::vec3 neworientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		if (abs(glm::angle(neworientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			orientation = neworientation;

		// Horizontal rotation
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, (width / 2), (height / 2));	// put the cursor back in the middle
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);		// unhides cursor
		firstClick = true;
	}
}

void Camera::setFOV(float fov) {
	this->fov = fov;
}

void Camera::setColliders(vector<Colliders> colliders) {
	this->colliders.clear();
	this->colliders = colliders;
}

void Camera::updateMatrix(float nearPlane, float farPlane) {
	view = glm::lookAt(position, (position + orientation), up);
	projection = glm::perspective(glm::radians(fov), (float)width / height, nearPlane, farPlane);

	matrix = projection * view;
}