#include "mesh.h"


Mesh::Mesh(vector<GLfloat> vertices, vector<GLuint> indices, vector<Texture> textures) {
	for (Texture t : textures) {
		if (t.getType() == "diffuse")
			tx.push_back(t);
		else if (t.getType() == "specular")
			spec.push_back(t);
	}

	this->vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
	this->ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));
	this->iSize = indices.size() * sizeof(int);
}

ObjectRectangular::ObjectRectangular() {
	GLfloat vertices[] = {
		//X		Y		Z		R	  G		B		TexL  TexU		LiX	  LiY	 LiZ
		// Bottom
		-0.5f,	0.0f,	0.5f,	0.8f, 0.3f, 0.3f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		// 0
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.3f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		// 1
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		// 2
		0.5f,	0.0f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,		// 3
		// Facing
		-0.5f,	0.0f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 0.0f,		0.0f, 0.5f,  0.8f,		// 4
		-0.5f,	0.8f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	0.0f, 0.5f,  0.8f,		// 5
		0.5f,	0.8f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		0.0f, 0.5f,  0.8f,		// 6
		0.5f,	0.0f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		0.0f, 0.5f,  0.8f,		// 7
		// Non-facing
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 0.0f,		0.0f, 0.5f, -0.8f,		// 8
		-0.5f,	0.8f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 1.0f,		0.0f, 0.5f, -0.8f,		// 9
		0.5f,	0.8f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 1.0f,		0.0f, 0.5f, -0.8f,		// 10
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 0.0f,		0.0f, 0.5f, -0.8f,		// 11
		// Left
		-0.5f,	0.0f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 0.0f,		-0.8f, 0.5f,  0.0f,		// 12
		-0.5f,	0.8f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	-0.8f, 0.5f,  0.0f,		// 13
		-0.5f,	0.8f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 1.0f,		-0.8f, 0.5f,  0.0f,		// 14
		-0.5f,	0.0f,	-0.5f,	0.1f, 0.3f, 0.5f,	1.0f, 0.0f,		-0.8f, 0.5f,  0.0f,		// 15
		// Right
		0.5f,	0.0f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 0.0f,		0.8f, 0.5f,  0.0f,		// 16
		0.5f,	0.8f,	-0.5f,	0.9f, 0.8f, 0.7f,	0.0f, 1.0f,		0.8f, 0.5f,  0.0f,		// 17
		0.5f,	0.8f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		0.8f, 0.5f,  0.0f,		// 18
		0.5f,	0.0f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		0.8f, 0.5f,  0.0f,		// 19
		// Top
		-0.5f,	0.8f,	0.5f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	-0.1f, 0.5f, 0.5f,		// 20
		-0.5f,	0.8f,	-0.5f,	0.1f, 0.3f, 0.5f,	0.0f, 0.0f,		-0.3f, 0.5f, 0.2f,		// 21
		0.5f,	0.8f,	-0.5f,	0.9f, 0.8f, 0.7f,	1.0f, 0.0f,		0.5f, 0.5f, -0.2f,		// 22
		0.5f,	0.8f,	0.5f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		0.0f, 0.5f, 0.8f		// 23
	};

	GLuint indices[] = {
		// Bottom
		0,1,2,
		0,2,3,
		// Facing
		4,5,6,
		4,6,7,
		// Non-facing
		8,9,10,
		8,10,11,
		// Left
		12,13,14,
		12,14,15,
		// Right
		16,17,18,
		16,18,19,
		// Top
		20,21,22,
		20,22,23
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}

ObjectCube::ObjectCube() {
	GLfloat vertices[] = {
		//X		Y		Z		R	  G		B		TexL  TexU
		// Bottom
		-0.3f,	-0.3f,	0.3f,	0.8f, 0.3f, 0.3f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		// 0
		-0.3f,	-0.3f,	-0.3f,	0.1f, 0.3f, 0.3f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		// 1
		0.3f,	-0.3f,	-0.3f,	0.9f, 0.8f, 0.7f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		// 2
		0.3f,	-0.3f,	0.3f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,		// 3
		// Facing
		-0.3f,	-0.3f,	0.3f,	0.8f, 0.5f, 0.3f,	0.0f, 0.0f,		0.0f, 0.5f,  0.8f,		// 4
		-0.3f,	0.3f,	0.3f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	0.0f, 0.5f,  0.8f,		// 5
		0.3f,	0.3f,	0.3f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		0.0f, 0.5f,  0.8f,		// 6
		0.3f,	-0.3f,	0.3f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		0.0f, 0.5f,  0.8f,		// 7
		// Non-facing
		-0.3f,	-0.3f,	-0.3f,	0.1f, 0.3f, 0.5f,	1.0f, 0.0f,		0.0f, 0.5f, -0.8f,		// 8
		-0.3f,	0.3f,	-0.3f,	0.1f, 0.3f, 0.5f,	1.0f, 1.0f,		0.0f, 0.5f, -0.8f,		// 9
		0.3f,	0.3f,	-0.3f,	0.9f, 0.8f, 0.7f,	0.0f, 1.0f,		0.0f, 0.5f, -0.8f,		// 10
		0.3f,	-0.3f,	-0.3f,	0.9f, 0.8f, 0.7f,	0.0f, 0.0f,		0.0f, 0.5f, -0.8f,		// 11
		// Left
		-0.3f,	-0.3f,	0.3f,	0.8f, 0.5f, 0.3f,	0.0f, 0.0f,		-0.8f, 0.5f,  0.0f,		// 12
		-0.3f,	0.3f,	0.3f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	-0.8f, 0.5f,  0.0f,		// 13
		-0.3f,	0.3f,	-0.3f,	0.1f, 0.3f, 0.5f,	1.0f, 1.0f,		-0.8f, 0.5f,  0.0f,		// 14
		-0.3f,	-0.3f,	-0.3f,	0.1f, 0.3f, 0.5f,	1.0f, 0.0f,		-0.8f, 0.5f,  0.0f,		// 15
		// Right
		0.3f,	-0.3f,	-0.3f,	0.9f, 0.8f, 0.7f,	0.0f, 0.0f,		0.8f, 0.5f,  0.0f,		// 16
		0.3f,	0.3f,	-0.3f,	0.9f, 0.8f, 0.7f,	0.0f, 1.0f,		0.8f, 0.5f,  0.0f,		// 17
		0.3f,	0.3f,	0.3f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		0.8f, 0.5f,  0.0f,		// 18
		0.3f,	-0.3f,	0.3f,	0.2f, 0.4f, 0.6f,	1.0f, 0.0f,		0.8f, 0.5f,  0.0f,		// 19
		// Top
		-0.3f,	0.3f,	0.3f,	0.8f, 0.5f, 0.3f,	0.0f, 1.0f, 	-0.1f, 0.5f, 0.5f,		// 20
		-0.3f,	0.3f,	-0.3f,	0.1f, 0.3f, 0.5f,	0.0f, 0.0f,		-0.3f, 0.5f, 0.2f,		// 21
		0.3f,	0.3f,	-0.3f,	0.9f, 0.8f, 0.7f,	1.0f, 0.0f,		0.5f, 0.5f, -0.2f,		// 22
		0.3f,	0.3f,	0.3f,	0.2f, 0.4f, 0.6f,	1.0f, 1.0f,		0.0f, 0.5f, 0.8f		// 23
	};

	GLuint indices[] = {
		// Bottom
		0,1,2,
		0,2,3,
		// Facing
		4,5,6,
		4,6,7,
		// Non-facing
		8,9,10,
		8,10,11,
		// Left
		12,13,14,
		12,14,15,
		// Right
		16,17,18,
		16,18,19,
		// Top
		20,21,22,
		20,22,23
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}

ObjectPyramid::ObjectPyramid() {
	GLfloat vertices[] = {
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,     -0.8f, 0.5f,  0.0f, // Left Side

		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.8f, 0.5f,  0.0f, // Right side

		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.0f, 0.5f,  0.8f  // Facing side
	};

	GLuint indices[] = {
		0, 1, 2, // Bottom side
		0, 2, 3, // Bottom side
		4, 6, 5, // Left side
		7, 9, 8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}

ObjectBlank::ObjectBlank() {
	GLfloat vertices[] = {
		-0.1f, -0.1f,  0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		-0.1f, -0.1f, -0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		 0.1f, -0.1f, -0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		 0.1f, -0.1f,  0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		-0.1f,  0.1f,  0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		-0.1f,  0.1f, -0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		 0.1f,  0.1f, -0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		 0.1f,  0.1f,  0.1f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0,1,2,
		0,2,3,
		0,4,7,
		0,7,3,
		3,7,6,
		3,6,2,
		2,6,5,
		2,5,1,
		1,5,4,
		1,4,0,
		4,5,6,
		4,6,7
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}

ObjectFlat::ObjectFlat() {
	GLfloat vertices[] = {
		-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 20.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		20.0f, 20.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		20.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}
