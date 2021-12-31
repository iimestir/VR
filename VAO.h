#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <vector>

#include "VBO.h"
#include "mesh.h"

using namespace std;
using namespace glm;

class VAO
{
private:
	GLuint ID;
	vector<Mesh> objects;

	mat4 matrix = mat4(1.0f);
	vec3 translation = vec3(0.0f, 0.0f, 0.0f);
	quat rotation = quat(0.0f, 0.0f, 0.0f, 0.0f);
	vec3 scaling = vec3(1.0f, 1.0f, 1.0f);

	float alpha = 1.0f;


	void linkAttributes(VBO&, GLuint, GLuint, GLenum, GLsizeiptr, void*);
	void linkMesh(Mesh);
public:
	VAO(bool = false);

	void addMesh(Mesh);

	void bind();
	void unbind();

	void draw();
	void bindMeshTextures();
	void registerMeshTextures(Shader& shader);

	void destroy();

	void setAlpha(float);

	inline float getAlpha() {
		return alpha;
	}

	inline mat4 getMatrix() {
		return matrix;
	}

	inline vec3 getTranslation() {
		return translation;
	}

	inline quat getRotation() {
		return rotation;
	}

	inline vec3 getScale() {
		return scaling;
	}

	inline void translate(float x, float y, float z) {
		translation.r = x;
		translation.g = y;
		translation.b = z;
	}

	inline void rotate(float w, float x, float y, float z) {
		rotation.w = w;
		rotation.x = x;
		rotation.y = y;
		rotation.z = z;
	}

	inline void scale(float x, float y, float z) {
		scaling.r = x;
		scaling.g = y;
		scaling.b = z;
	}

	inline GLuint getID() {
		return VAO::ID;
	}

	inline Mesh getMesh(unsigned index) {
		return objects.at(index);
	}

	inline size_t getMeshAmount() {
		return objects.size();
	}
};
#endif