#ifndef VERTICE_H
#define VERTICE_H

#include <glad/glad.h>

class Vertice
{
private:
	GLfloat* coordinates;
	GLfloat* colors;
	GLfloat* texCorners;

public:
	Vertice(GLfloat x, GLfloat y, GLfloat z, GLfloat red, GLfloat green, GLfloat blue, GLfloat texLower, GLfloat texUpper);

	inline GLfloat* getCoords() {
		return Vertice::coordinates;
	}

	inline GLfloat* getColors() {
		return Vertice::colors;
	}

	inline GLfloat* getTexCorners() {
		return Vertice::texCorners;
	}
};
#endif