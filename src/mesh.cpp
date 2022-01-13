#include "headers/mesh.h"


Mesh::Mesh(vector<GLfloat> vertices, vector<GLuint> indices, vector<Texture> textures, bool collisions) {
	// Textures
	for (Texture t : textures) {
		if (t.getType() == "tex0")
			diffuses.push_back(t);
		else if (t.getType() == "tex1")
			speculars.push_back(t);
		else if (t.getType() == "tex2")
			normals.push_back(t);
		else if (t.getType() == "tex3")
			parallaxes.push_back(t);
	}

	float minX = 0.0f;
	float minY = 0.0f;
	float minZ = 0.0f;

	float maxX = 0.0f;
	float maxY = 0.0f;
	float maxZ = 0.0f;

	bool initiated = false;

	// Colliders
	for (unsigned i = 0; i < vertices.size(); i+=11) {
		if (!initiated) {
			minX = vertices.at(i);
			minY = vertices.at(i + 1);
			minZ = vertices.at(i + 2);
			maxX = vertices.at(i);
			maxY = vertices.at(i + 1);
			maxZ = vertices.at(i + 2);

			initiated = true;
		}
		else {
			minX = fmin(minX, vertices.at(i));
			minY = fmin(minY, vertices.at(i + 1));
			minZ = fmin(minZ, vertices.at(i + 2));
			maxX = fmax(maxX, vertices.at(i));
			maxY = fmax(maxY, vertices.at(i + 1));
			maxZ = fmax(maxZ, vertices.at(i + 2));
		}
	}

	if(collisions)
		this->colliders = { minX - 0.14f, minY - 0.14f, minZ - 0.14f, maxX + 0.14f, maxY + 0.14f, maxZ + 0.14f };
	this->vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
	this->ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));
	this->iSize = indices.size() * sizeof(int);
}

void Mesh::setTexture(const char* diff) {
	diffuses.push_back(Texture(diff, "tex0", 0));
}

void Mesh::setSpecular(const char* spec) {
	speculars.push_back(Texture(spec, "tex1", 1));
}

void Mesh::setNormal(const char* nrm) {
	normals.push_back(Texture(nrm, "tex2", 1));
}

void Mesh::setBump(const char* par) {
	parallaxes.push_back(Texture(par, "tex3", 2));
}

void Mesh::registerTextures(Shader& shader) {
	for (Texture diff : diffuses)
		diff.registerTexture(shader, "tex0", 0);

	for (Texture spec : speculars)
		spec.registerTexture(shader, "tex1", 1);

	for (Texture nrm : normals)
		nrm.registerTexture(shader, "tex2", 1);

	for (Texture par : parallaxes)
		par.registerTexture(shader, "tex3", 2);
}

void Mesh::bind() {
	vbo->bind();
	ebo->bind();
}

void Mesh::unbind() {
	vbo->unbind();
	ebo->unbind();
}

void Mesh::destroy() {
	unbind();

	vbo->deleteBO();
	ebo->deleteBO();

	free(vbo);
	free(ebo);
	diffuses.clear();
	speculars.clear();
	normals.clear();
}

bool Mesh::collidesWith(float x, float y, float z) {
	return colliders.collidesWith(x, y, z);
}

bool Mesh::collidesWith(vec3 pos) {
	return collidesWith(pos.r, pos.g, pos.b);
}

void Mesh::setCollidersAction(function<void(unsigned)> f, unsigned id) {
	colliders.handle = f;
	colliders.vaoID = id;
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

	float minX = 0.0f;
	float minY = 0.0f;
	float minZ = 0.0f;

	float maxX = 0.0f;
	float maxY = 0.0f;
	float maxZ = 0.0f;

	bool initiated = false;

	// Colliders
	for (unsigned i = 0; i < (sizeof(vertices) / sizeof(*vertices)); i += 11) {
		if (!initiated) {
			minX = vertices[i];
			minY = vertices[i + 1];
			minZ = vertices[i + 2];
			maxX = vertices[i];
			maxY = vertices[i + 1];
			maxZ = vertices[i + 2];

			initiated = true;
		}
		else {
			minX = fmin(minX, vertices[i]);
			minY = fmin(minY, vertices[i + 1]);
			minZ = fmin(minZ, vertices[i + 2]);
			maxX = fmax(maxX, vertices[i]);
			maxY = fmax(maxY, vertices[i + 1]);
			maxZ = fmax(maxZ, vertices[i + 2]);
		}
	}

	this->colliders = { minX - 0.14f, minY - 0.14f, minZ - 0.14f, maxX + 0.14f, maxY + 0.14f, maxZ + 0.14f };
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

ObjectEmpty::ObjectEmpty() {
	GLfloat vertices[] = {
		0.0f, 0.0f,  0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0
	};

	this->vbo = new VBO(vertices, sizeof(vertices));
	this->ebo = new EBO(indices, sizeof(indices));
	this->iSize = sizeof(indices);
}