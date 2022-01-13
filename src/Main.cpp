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
#include "headers/game.h"

using namespace std;
using namespace glm;

int main() {
	// Sounds setup
	Audio::getInstance().playMusic("sounds/music.wav", 12.0f);
	Audio::getInstance().pushSound("sounds/step1.wav");
	Audio::getInstance().pushSound("sounds/step2.wav");
	Audio::getInstance().pushSound("sounds/step3.wav");
	Audio::getInstance().pushSound("sounds/step4.wav");
	Audio::getInstance().pushSound("sounds/step5.wav");
	Audio::getInstance().pushSound("sounds/step6.wav");
	Audio::getInstance().pushSound("sounds/step7.wav");
	Audio::getInstance().pushSound("sounds/step8.wav");
	Audio::getInstance().pushSound("sounds/paranormal1.wav");
	Audio::getInstance().pushSound("sounds/paranormal2.wav");
	Audio::getInstance().pushSound("sounds/paranormal3.wav");
	Audio::getInstance().pushSound("sounds/paranormal4.wav");
	Audio::getInstance().pushSound("sounds/paranormal5.wav");
	Audio::getInstance().pushSound("sounds/paper.wav");
	Audio::getInstance().pushSound("sounds/end.wav");

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
	GLFWwindow* window = glfwCreateWindow(width, height, "Pitch Black Maze by IMESTIR Ibrahim and BELGADA Wassim", NULL, NULL);
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
		"textures/skybox/starnight/right.jpg",
		"textures/skybox/starnight/left.jpg",
		"textures/skybox/starnight/top.jpg",
		"textures/skybox/starnight/bottom.jpg",
		"textures/skybox/starnight/front.jpg",
		"textures/skybox/starnight/back.jpg"
	};

	// Generates shader object using vShader and fShader files
	Game game = Game();
	Scene scene("shaders/world.vs", "shaders/world.fs", "shaders/world.gs", width, height, sky);
	Camera camera(width, height, vec3(0.0f, 0.0f, 3.0f), 80.0f, 0.015f, 100.0f);
	scene.setBackgroundColor(window, width, height, 0.0f, 0.0f, 0.0f, 0.0f);
	scene.setPPType(PPType::DEFAULT);

	scene.registerObserver(&game);

	// Load our hand-made maze by blender
	vector<unsigned> maze = scene.loadMesh("models/maze/maze.obj", true);
	vector<unsigned> notes = scene.loadMesh("models/notes/note.obj", true, true);
	vector<unsigned> fence = scene.loadMesh("models/fence/fence.obj", true);

	game.setDoorID(scene.getVertexID(fence[0]));

	scene.addText("Obtained notes 0/8", width, height, width * 0.75f, height * 0.9f, 0.4f, 0.2f, 0.65f, 0.5f);

	// flash light
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		DFloat(camera.getPPosX()), DFloat(camera.getPPosY()), DFloat(camera.getPPosZ()),
		DFloat(camera.getPOriX()), DFloat(camera.getPOriY()), DFloat(camera.getPOriZ()),
		0.75, 0.75, 0.75, 1.0f)
	);

	// Red lights
	scene.addLight(Light(ObjectBlank(), "shaders/light.vs", "shaders/light.fs", LightType::SOURCE,
		0.0f, 1.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.8f, 0.0f, 0.0f, 1.0f)
	);

	// White Lights
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		8.78f, 1.0f, 10.64f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		8.87f, 1.0f, 37.89f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		8.87f, 1.0f, 53.76f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-0.15f, 1.0f, 70.38f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-16.35f, 1.0f, 57.02f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-14.72f, 1.0f, 41.59f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-51.64f, 1.0f, 53.43f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-68.62f, 1.0f, 54.54f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-79.40f, 1.0f, 71.0f,
		0.0f, -1.0f, 0.0f,
		0.88f, 0.88f, 0.88f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-66.43f, 1.0f, 113.51f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.8f, 0.0f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-63.40f, 1.0f, 113.51f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.8f, 0.0f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-8.4f, 1.0f, 94.34f,
		0.0f, -1.0f, 0.0f,
		0.8f, 0.8f, 0.8f, 1.0f)
	);
	scene.addLight(Light(ObjectEmpty(), "shaders/light.vs", "shaders/light.fs", LightType::SPOT,
		-100.f, 1.0f, 23.34f,
		0.0f, -1.0f, 0.0f,
		0.8f, 0.8f, 0.8f, 1.0f)
	);

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