#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"
#include "camera.h"

using namespace std;
using namespace glm;

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
		cout << "An error has occured" << endl;

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	/*
	DRAWING

	Shader <- VAO <- Object3D (VBO + EBO) <- Texture 

	*/

	// Generates shader object using vShader and fShader files
	Scene scene("default.vert", "default.frag");
	Camera camera(width, height, vec3(0.0f, 0.0f, 2.0f), 0.01f, 100.0f);
	scene.setBackgroundColor(window, width, height, 0.07f, 0.13f, 0.17f);

	unsigned objectIndex = scene.addObject3D(
		ObjectCube("o.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, *scene.getShader(), "tex0", 0),
		0.0f, 0.0f, 0.0f
	);
	unsigned lightIndex = scene.addLight(ObjectLittleCube(), "light.vert", "light.frag", 0.5f, 0.5f, 0.5f);

	scene.setLightShaderColor(lightIndex, 1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	/*
	END DRAWING
	*/
	while (!glfwWindowShouldClose(window)) {
		scene.setGLColor(0.07f, 0.13f, 0.17f);

		camera.defineInputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		scene.setCameraMatrix(&camera, "camera");

		scene.draw();

		glfwSwapBuffers(window);						// Swap the back and front buffer
		glfwPollEvents();								// Takes care of all GLFW events
	}

	// End
	scene.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}