#include "vertice.h"

Vertice::Vertice(GLfloat x, GLfloat y, GLfloat z, GLfloat red, GLfloat green, GLfloat blue, GLfloat texLower, GLfloat texUpper) {
	GLfloat coords[] = {x,y,z};
	GLfloat colors[] = {red,green,blue};
	GLfloat texCoords[] = {texLower,texUpper};
	
	this->coordinates = coords;
	this->colors = colors;
	this->texCorners = texCoords;
}
