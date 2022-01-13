#ifndef SCENE_H
#define SCENE_H

# define PI				3.14159265358979323846
# define LOADER_FLAGS	aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices

#include <glad/glad.h>
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VAO.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "postProcess.h"
#include "dFloat.h"
#include "skyBox.h"
#include "subject.h"
#include "textGui.h"

using namespace std;
using namespace glm;

class Scene : public Subject {
private:
	Shader program;
	vector<VAO> vertices;
	vector<Light> lights;
	vector<TextGUI> texts;

	vector<Colliders> colliders;

	PostProcess pp;
	SkyBox sb;

	vec4 depthColor;

	unsigned loadedFiles = 0;
	map<unsigned, map<unsigned, vector<Texture>>> loadedMaterials;

	void registerVertexOnShader(Shader&, unsigned);
	void registerLightOnShader(unsigned);

	void updateLightsUni();
	void resetLightsUni();

	void translateOnShader(Shader&, unsigned, float, float, float, bool = false);
	void rotateOnShader(Shader&, unsigned, float, float, float, float);
	void scaleOnShader(Shader&, unsigned, float, float, float);
	void alphaOnShader(Shader&, float);

	void setDepthUniform(Shader&);

	vector<Texture> retrieveMeshTextures(const aiScene*, aiMesh*, const char*);
	Mesh retrieveMesh(const aiScene*, aiMesh*, const char*, bool = false);

	void notifyCameraPosition(Camera*);

	void draw(Camera*, unsigned, unsigned);
	void drawVertices(Camera*, unsigned, unsigned);
	void drawLights(Camera*, unsigned, unsigned);
	void drawUnLightedObjects(Camera*, unsigned, unsigned);
	void drawTexts();

	void updateColliders();
public:
	Scene(const char*, const char*, const char*, unsigned, unsigned, vector<string>);

	void render(GLFWwindow*, Camera*, unsigned, unsigned);

	void activateShader();
	void activateLightShader(unsigned);

	void destroy();

	void setGLColor(GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);
	void setBackgroundColor(GLFWwindow*, unsigned int, unsigned int, GLfloat, GLfloat, GLfloat, GLfloat = 1.0f);
	void setPPType(PPType);

	unsigned addMesh(Mesh, bool = false, float = 0.0f, float = 0.0f, float = 0.0f, float = 1.0f);
	vector<unsigned> loadMesh(const char*, bool = false, bool = false);
	unsigned addLight(Light);

	unsigned addText(const char*, unsigned, unsigned, float, float, float = 1.0f, float = 1.0f, float = 1.0f, float = 1.0f);
	void editText(unsigned, string);
	void editTextColor(unsigned, float = 1.0f, float = 1.0f, float = 1.0f);

	void bindVertexPosition(unsigned, DFloat, DFloat, DFloat);
	void bindVertexOrientation(unsigned, DFloat, DFloat, DFloat);

	void translateVertex(unsigned, float, float, float);
	void rotateVertex(unsigned, float, float, float);
	void scaleVertex(unsigned, float, float, float);

	void setLightColor(unsigned, float, float, float, float);

	void setCameraMatrix(Camera*);

	void destroyVertex(unsigned);
	void destroyVertexbyID(unsigned);
	void removeText(unsigned);

	bool collidesWith(float, float, float);
	bool collidesWith(vec3);

	void enableCulling();
	void disableCulling();

	unsigned getVertexID(unsigned);

	inline Shader* getShader() {
		return &program;
	}

	inline Shader* getLightShader(unsigned index) {
		return &lights.at(index).getShader();
	}

	inline GLuint getShaderID() {
		return program.getID();
	}

	vector<Colliders>* getColliders() {
		return &colliders;
	};
};

#endif
