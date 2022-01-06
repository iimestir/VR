#include "headers/camera.h"

Camera::Camera(int width, int height, glm::vec3 position, float fov, float speed, float sensitivity) {
	this->width = width;
	this->height = height;
	this->position = position;
	this->speed = speed;
	this->sensitivity = sensitivity;
	this->initialSpeed = speed;
	this->initialSensitivity = sensitivity;
	this->fov = fov;
}

void Camera::sendMatrixToShader(Shader& shader) {
	glUniformMatrix4fv(shader.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Camera::defineInputs(GLFWwindow* window) {
	// Keyboard inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * orientation;
		camera_time += 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += speed * -glm::normalize(glm::cross(orientation, up));
		camera_time -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position += speed * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.5*initialSpeed;

		cout << position.r << " " << position.g << " " << position.b << " " << endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = initialSpeed;
	}

	// Mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
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

void Camera::updateMatrix(float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, (position + orientation), up);
	projection = glm::perspective(glm::radians(fov), (float)width / height, nearPlane, farPlane);

	matrix = projection * view;
}