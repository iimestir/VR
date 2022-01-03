#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <vector>

#include "VBO.h"
#include "mesh.h"
#include "dFloat.h"

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

	vector<DFloat> posiBinds;
	vector<DFloat> oriBinds;
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
	void unbindMeshTextures();
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

	void translate(float, float, float);
	void rotate(float, float, float, float);
	void rotateEuler(float, float, float);
	void scale(float, float, float);

	void bindPositionTo(DFloat, DFloat, DFloat);
	void bindRotationTo(DFloat, DFloat, DFloat);
	void clearBounds();

	inline vector<DFloat> getPositionBounds() {
		return posiBinds;
	}

	inline bool hasPositionBounds() {
		return !posiBinds.empty();
	}

	inline vector<DFloat> getOrientationBounds() {
		return oriBinds;
	}

	inline bool hasOrientationBounds() {
		return !oriBinds.empty();
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