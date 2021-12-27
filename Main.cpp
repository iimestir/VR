#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "object3D.h"

using namespace std;

void setGLColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f) {
	// "Adds" a color to the back buffer
	glClearColor(red, green, blue, alpha);
	// Clears what's in the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void setBackgroundColor(GLFWwindow * window, unsigned int width, unsigned int height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f) {
	// Creates the work zone
	glViewport(0, 0, width, height);
	setGLColor(red, green, blue, alpha);

	// Swaps the front and back buffers
	glfwSwapBuffers(window);
}

int main() {
	/*
	SETUP
	*/
	const unsigned width = 1280;
	const unsigned height = 720;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates the GL window
	GLFWwindow* window = glfwCreateWindow(width, height, "VR sandbox", NULL, NULL);
	if (window == NULL) {
		std::cout << "An error has occured" << std::endl;

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	setBackgroundColor(window, width, height, 0.07f, 0.13f, 0.17f);

	/*
	DRAWING

	Shader <- VAO <- Object3D (VBO + EBO) <- Texture 

	*/

	// Generates shader object using vShader and fShader files
	Shader program("default.vert", "default.frag");

	// Generates a Vertex Array Object and binds it
	VAO vao(true);

	Object3D cube = ObjectTriangle();
	cube.setTexture("bidoof.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, program, "tex0", 0);

	// Link the VBO to the VAO
	vao.linkObject3D(cube);

	// Unbind everything to be sure we won't modify it accidentally
	vao.unbind();
	cube.unbind();

	// Uniform scale
	GLuint uniID = glGetUniformLocation(program.getID(), "scale");

	float rotation = 0.0f;
	double previousTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	/*
	END DRAWING
	*/
	while (!glfwWindowShouldClose(window)) {
		setGLColor(0.07f, 0.13f, 0.17f);

		program.activateShader();

		double currentTime = glfwGetTime();
		if (currentTime - previousTime >= 1 / 60) {
			rotation += 0.5f;
			previousTime = currentTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -4.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		// Fragment Uniform variables
		int modelLocation = glGetUniformLocation(program.getID(), "model");
		int viewLocation = glGetUniformLocation(program.getID(), "view");
		int projLocation = glGetUniformLocation(program.getID(), "proj");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1f(uniID, 0.5f);
		cube.bindTexture();
		vao.bind();

		glDrawElements(GL_TRIANGLES, cube.getISize(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);						// Swap the back and front buffer
		glfwPollEvents();								// Takes care of all GLFW events
	}

	// End
	vao.deleteAO();
	cube.destroy();
	program.deleteShader();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}