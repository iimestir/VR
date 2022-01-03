#ifndef LIGHT_H
#define LIGHT_H

#include "VAO.h"

enum LightType {
	SPOT = 0, POINT = 1, DIRECT = 2
};

class Light {
private:
	float r;
	float g;
	float b;

	VAO vao;
	Shader shader;

	LightType type;

public:
	Light(Mesh obj, const char* vFile, const char* fFile, LightType type, 
		float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f,
		float oriX = 0.0f, float oriY = -1.0f, float oriZ = 0.0f,
		float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f
	);
	Light(Mesh obj, const char* vFile, const char* fFile, LightType type, 
		DFloat = DFloat(NULL, 0.0f), DFloat = DFloat(NULL, 0.0f), DFloat = DFloat(NULL, 0.0f),
		DFloat = DFloat(NULL, 0.0f), DFloat = DFloat(NULL, -1.0f), DFloat = DFloat(NULL, 0.0f),
		float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f
	);

	inline quat getOrientation() {
		return vao.getRotation();
	}

	inline vector<DFloat> getOrientationBounds() {
		return vao.getOrientationBounds();
	}

	inline bool hasOrientationBounds() {
		return vao.hasOrientationBounds();
	}

	inline float getR() {
		return r;
	}

	inline float getG() {
		return g;
	}

	inline float getB() {
		return b;
	}

	inline float getAlpha() {
		return vao.getAlpha();
	}

	inline Shader& getShader() {
		return shader;
	}

	inline VAO& getVAO() {
		return vao;
	}

	inline LightType getType() {
		return type;
	}
};

#endif