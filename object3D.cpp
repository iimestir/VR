#include "object3D.h"

ObjectCuboid::ObjectCuboid() {
	GLfloat vertices[] = {
		/*
		X		Y		Z		R	  G		B		TexL  TexU
		*/
		-0.5f,	0.0f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 0.0f, 	// FLOOR Lower left corner	->  0
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 0.0f,		// FLOOR Upper left corner	->	1
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 0.0f,		// FLOOR Upper right corner	->	2
		0.5f,	0.0f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		// FLOOR Lower right corner	->	3

		-0.5f,	0.8f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	// CEILING Lower left corner	->  4
		-0.5f,	0.8f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 1.0f,		// CEILING Upper left corner	->	5
		0.5f,	0.8f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 1.0f,		// CEILING Upper right corner	->	6
		0.5f,	0.8f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		// CEILING Lower right corner	->	7

		// Duplicates to precise other TexL TexU for some points
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.5f,	0.0f, 1.0f,		// FLOOR Upper left corner	->	8
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	1.0f, 1.0f,		// FLOOR Upper right corner	->	9
		-0.5f,	0.8f,	-0.5f,	0.1f, 0.3f, 0.5f,	0.0f, 0.0f,		// CEILING Upper left corner	->	10
		0.5f,	0.8f,	-0.5f,	0.9f, 0.8f, 0.7f,	1.0f, 0.0f,		// CEILING Upper right corner	->	11
	};

	GLuint indices[] = {
		// FLOOR
		0,8,9,
		0,9,3,
		// FACES
		0,4,3,
		3,4,7,
		//
		0,4,1,
		1,4,5,
		//
		2,6,3,
		3,6,7,
		//
		2,6,1,
		1,6,5,
		// CEILING
		4,10,11,
		4,7,11
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}

ObjectCube::ObjectCube() {
	GLfloat vertices[] = {
		/*
		X		Y		Z		R	  G		B		TexL  TexU
		*/
		-0.5f,	0.0f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 0.0f, 	// FLOOR Lower left corner	->  0
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 0.0f,		// FLOOR Upper left corner	->	1
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 0.0f,		// FLOOR Upper right corner	->	2
		0.5f,	0.0f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		// FLOOR Lower right corner	->	3

		-0.5f,	1.5f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	// CEILING Lower left corner	->  4
		-0.5f,	1.5f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 1.0f,		// CEILING Upper left corner	->	5
		0.5f,	1.5f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 1.0f,		// CEILING Upper right corner	->	6
		0.5f,	1.5f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		// CEILING Lower right corner	->	7

		// Duplicates to precise other TexL TexU for some points
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.5f,	0.0f, 1.0f,		// FLOOR Upper left corner	->	8
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	1.0f, 1.0f,		// FLOOR Upper right corner	->	9
		-0.5f,	1.5f,	-0.5f,	0.1f, 0.3f, 0.5f,	0.0f, 0.0f,		// CEILING Upper left corner	->	10
		0.5f,	1.5f,	-0.5f,	0.9f, 0.8f, 0.7f,	1.0f, 0.0f,		// CEILING Upper right corner	->	11
	};

	GLuint indices[] = {
		// FLOOR
		0,8,9,
		0,9,3,
		// FACES
		0,4,3,
		3,4,7,
		//
		0,4,1,
		1,4,5,
		//
		2,6,3,
		3,6,7,
		//
		2,6,1,
		1,6,5,
		// CEILING
		4,10,11,
		4,7,11
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}

ObjectTriangle::ObjectTriangle() {
	GLfloat vertices[] = {
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}