#ifndef MODEL_H
#define MODEL_H

#include <json/json.h>
#include <vector>

#include "mesh.h"
#include "camera.h"

using json = nlohmann::json;
using namespace std;
using namespace glm;

class Model {
private:
	const char* file;
	vector<unsigned char> data;
	json JSON;

	vector<Mesh> meshes;

	vector<vec3> translationsMeshes;
	vector<quat> rotationsMeshes;
	vector<vec3> scalesMeshes;
	vector<mat4> matricesMeshes;

	vector<string> loadedTexName;
	vector<Texture> loadedTex;

	vector<unsigned char> getData();
	vector<float> getFloats(json);
	vector<GLuint> getIndices(json);

	vector<vec2> groupFloatsV2(vector<float>);
	vector<vec3> groupFloatsV3(vector<float>);
	vector<vec4> groupFloatsV4(vector<float>);

	vector<GLfloat> assembleVertices(vector<vec3>, vector<vec2>, vector <vec3>);
	void loadMesh(unsigned int);

	void traverseNode(unsigned int, mat4 = mat4(1.0));

	vector<Texture> getTextures();

public:
	Model(const char*);

	vector<Mesh> getMeshes() {
		return meshes;
	}
};


#endif