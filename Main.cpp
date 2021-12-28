#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"
#include "camera.h"
#include "model.h"

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

	Model model("models/sword/scene.gltf");

	unsigned sword;

	for (int i = 0; i < model.getMeshes().size(); i++)
		sword = scene.addMesh(model.getMeshes().at(i), 0.0f, 0.0f, 0.0f);

	scene.scaleOnGlobal(sword, 0.1f, 0.1f, 0.1f);

	scene.addMesh(
		ObjectRectangular("o.jpg", NULL),
		0.0f, 0.0f, 0.0f
	);

	/*
	scene.addMesh(
		ObjectCube("o.jpg", NULL),
		0.0f, 0.0f, 0.0f
	);
	scene.addMesh(
		ObjectPyramid("bidoof.jpg", NULL),
		0.0f, 0.301f, 0.0f
	);
	scene.addMesh(
		ObjectFlat("grass.jpg", "spec_grass.jpg"),
		0.0f, -0.301f, 0.0f
	);

	scene.addLight(ObjectBlank(), "light.vert", "light.frag", 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);
	*/

	glEnable(GL_DEPTH_TEST);

	/*
	END DRAWING
	*/
	while (!glfwWindowShouldClose(window)) {
		scene.render(window, &camera);
	}

	// End
	scene.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}