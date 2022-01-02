#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/scene.h"
#include "headers/camera.h"
#include "headers/light.h"
#include "headers/postProcess.h"

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
	glViewport(0, 0, width, height);

	/*
	DRAWING SETUP

	Shader <- VAO <- Object3D (VBO + EBO) <- Texture 

	*/

	// Generates shader object using vShader and fShader files
	Scene scene("shaders/default.vs", "shaders/default.fs", width, height);
	Camera camera(width, height, vec3(0.0f, 0.0f, 2.0f), 0.005f, 100.0f);
	scene.setBackgroundColor(window, width, height, 0.0f, 0.0f, 0.0f, 0.0f);
	scene.setPPType(PPType::OUTLINE);

	scene.loadMesh("models/colt/colt.obj");
	vector<unsigned> flashlight = scene.loadMesh("models/flashlight/Flashilght.obj");

	//scene.scaleVertex(creeper, 0.035f, 0.05f, 0.05f);
	//scene.translateVertex(creeper, -0.5f, -0.25f, 0.5f);
	//scene.rotateVertex(creeper, 0.0f, 90.0f, 0.0f);

	/*
	scene.addMesh(
		ObjectCube("textures/diffuse/o.jpg", NULL),
		0.0f, 0.0f, 0.0f
	);
	scene.addMesh(
		ObjectPyramid("textures/diffuse/bidoof.jpg", NULL),
		0.0f, 0.301f, 0.0f
	);*/
	scene.addMesh(
		ObjectFlat("textures/diffuse/grass.jpg", "textures/specular/spec_grass.jpg"),
		0.0f, -0.301f, 0.0f
	);

	for (unsigned i : flashlight) {
		scene.scaleVertex(i, 0.05f, 0.05f, 0.05f);
		scene.bindVertexPosition(i, camera.getPPosX(), camera.getPPosY(), camera.getPPosZ());
		scene.bindVertexOrientation(i, camera.getPOriX(), camera.getPOriY(), camera.getPOriZ());
	}

	/*
	scene.addLight(Light(ObjectBlank(), "shaders/light.vs", "shaders/light.fs", 
		LightType::POINT, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f));*/
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		camera.getPPosX(), camera.getPPosY(), camera.getPPosZ(),
		camera.getPOriX(), camera.getPOriY(), camera.getPOriZ(), 
		1.0f, 1.0f, 1.0f, 1.0f));

	/*
	END DRAWING SETUP
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