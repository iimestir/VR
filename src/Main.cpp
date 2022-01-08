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
#include "headers/audio.h"

using namespace std;
using namespace glm;

int main() {
	// Sounds setup
	Audio::getInstance().playMusic("sounds/music.wav", 12.0f);
	Audio::getInstance().pushSound("sounds/step1.wav");
	Audio::getInstance().pushSound("sounds/step2.wav");

	/*
	SETUP
	*/
	const unsigned width = 1920;
	const unsigned height = 1080;

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
	vector<std::string> sky {
		"textures/skybox/night/right.jpg",
		"textures/skybox/night/left.jpg",
		"textures/skybox/night/top.jpg",
		"textures/skybox/night/bottom.jpg",
		"textures/skybox/night/front.jpg",
		"textures/skybox/night/back.jpg"
	};

	// Generates shader object using vShader and fShader files
	Scene scene("shaders/world.vs", "shaders/world.fs", width, height, sky);
	Camera camera(width, height, vec3(0.0f, 0.0f, 3.0f), 80.0f, 0.015f, 100.0f);
	scene.setBackgroundColor(window, width, height, 0.0f, 0.0f, 0.0f, 0.0f);
	scene.setPPType(PPType::OUTLINE);

	/*
	vector<unsigned> colt = scene.loadMesh("models/colt/colt.obj", true);
	vector<unsigned> flashlight = scene.loadMesh("models/flashlight/Flashlight.obj");
	unsigned object = scene.addMesh(ObjectRectangular("textures/diffuse/o.jpg", NULL),0.0f, -0.3f, 0.0f);
	scene.translateVertex(object, 2.0f, -0.3f, 0.0f);
	scene.addLight(Light(ObjectBlank(), "shaders/light.vs", "shaders/light.fs", 
		LightType::POINT, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f));
	*/

	// Load our hand-made maze by blender
	scene.loadMesh("models/maze/maze.obj", true);

	// flash light
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		DFloat(camera.getPPosX()), DFloat(camera.getPPosY()), DFloat(camera.getPPosZ()),
		DFloat(camera.getPOriX()), DFloat(camera.getPOriY()), DFloat(camera.getPOriZ()),
		1.0f, 1.0f, 1.0f, 1.0f));

	// Collisions
	camera.setColliders(scene.getColliders());

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