#include "headers/light.h"

Light::Light(Mesh obj, const char* vFile, const char* fFile, LightType type,
	float posX, float posY, float posZ,
	float oriX, float oriY, float oriZ,
	float red, float green, float blue, float alpha)
	: shader(vFile, fFile) {
	vao = VAO(true);
	vao.addMesh(obj);
	vao.unbind();

	r = red;
	g = green;
	b = blue;
	vao.setAlpha(alpha);
	this->type = type;

	vao.translate(posX, posY, posZ);
	vao.rotateEuler(oriX, oriY, oriZ);
}

Light::Light(Mesh obj, const char* vFile, const char* fFile, LightType type,
	DFloat dynPosX, DFloat dynPosY, DFloat dynPosZ,
	DFloat dynOriX, DFloat dynOriY, DFloat dynOriZ,
	float red, float green, float blue, float alpha)
	: shader(vFile, fFile) {
	vao = VAO(true);
	vao.addMesh(obj);
	vao.unbind();

	r = red;
	g = green;
	b = blue;
	vao.setAlpha(alpha);
	this->type = type;

	vao.bindPositionTo(dynPosX, dynPosY, dynPosZ);
	vao.bindRotationTo(dynOriX, dynOriY, dynOriZ);
}
