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

	// SkyBox
	// https://jaxry.github.io/panorama-to-cubemap/
	vector<std::string> sky
	{
		"textures/skybox/night/right.jpg",
		"textures/skybox/night/left.jpg",
		"textures/skybox/night/top.jpg",
		"textures/skybox/night/bottom.jpg",
		"textures/skybox/night/front.jpg",
		"textures/skybox/night/back.jpg"
	};

	// Generates shader object using vShader and fShader files
	Scene scene("shaders/world.vs", "shaders/world.fs", width, height, sky);
	Camera camera(width, height, vec3(0.0f, 0.0f, 2.0f), 80.0f, 0.005f, 100.0f);
	scene.setBackgroundColor(window, width, height, 0.0f, 0.0f, 0.0f, 0.0f);
	scene.setPPType(PPType::DEFAULT);

	//vector<unsigned> colt = scene.loadMesh("models/colt/colt.obj");
	vector<unsigned> flashlight = scene.loadMesh("models/flashlight/Flashlight.obj");

	scene.addMesh(
		ObjectFlat("textures/diffuse/grass.jpg", "textures/specular/spec_grass.jpg"),
		0.0f, -0.301f, 0.0f
	);

	for (unsigned i : flashlight) {
		scene.scaleVertex(i, 0.05f, 0.05f, 0.05f);
		//scene.bindVertexPosition(i, DFloat(camera.getPPosX()), DFloat(camera.getPPosY()), DFloat(camera.getPPosZ()));
		scene.bindVertexOrientation(i, DFloat(camera.getPOriX()), DFloat(camera.getPOriY()), DFloat(camera.getPOriZ()));
	}

	/*
	scene.addLight(Light(ObjectBlank(), "shaders/light.vs", "shaders/light.fs", 
		LightType::POINT, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f));*/

	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		DFloat(camera.getPPosX()), DFloat(camera.getPPosY()), DFloat(camera.getPPosZ()),
		DFloat(camera.getPOriX()), DFloat(camera.getPOriY()), DFloat(camera.getPOriZ()),
		1.0f, 1.0f, 1.0f, 1.0f));

	/*
	END DRAWING SETUP
	*/
	while (!glfwWindowShouldClose(window)) {
		scene.render(window, &camera, width, height);
	}

	// End
	scene.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}