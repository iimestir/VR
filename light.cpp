#include "light.h"

Light::Light(Mesh obj, const char* vFile, const char* fFile, LightType type, float posX, float posY, float posZ, float red, float green, float blue, float alpha) : shader(vFile, fFile) {
	vao = VAO(true);
	vao.addMesh(obj);
	vao.unbind();

	r = red;
	g = green;
	b = blue;
	vao.setAlpha(alpha);
	this->type = type;

	vao.translate(posX, posY, posZ);
}