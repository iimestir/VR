#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <vector>

#include "VBO.h"
#include "object3D.h"

using namespace std;

class VAO
{
private:
	GLuint ID;
	vector<Object3D> objects;

	void linkAttributes(VBO&, GLuint, GLuint, GLenum, GLsizeiptr, void*);
	void linkObject3D(Object3D);
public:
	VAO(bool = false);

	void addObject3D(Object3D);

	void bind();
	void unbind();

	void draw();
	void bindObject3DTextures();

	void destroy();
	
	inline GLuint getID() {
		return VAO::ID;
	}

	inline Object3D getObject3D(unsigned index) {
		return objects.at(index);
	}
};
#endif