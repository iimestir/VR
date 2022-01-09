#include "headers/VAO.h"

VAO::VAO(bool bind) {
	glGenVertexArrays(1, &ID);

	if (bind)
		VAO::bind();
}

void VAO::addMesh(Mesh obj) {
	linkMesh(obj);
	this->objects.push_back(obj);
}

void VAO::linkAttributes(VBO& VBO, GLuint layout, GLuint nComp, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.bind();

	// the stride is the distance in bytes between the beginning and the end of a vertex
	// offset is the offset to get the color values in the vertex (offsetting the coordinates values)

	glVertexAttribPointer(layout, nComp, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	VBO.unbind();
}

void VAO::linkMesh(Mesh obj) {
	linkAttributes(*obj.getVBO(), 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	linkAttributes(*obj.getVBO(), 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	linkAttributes(*obj.getVBO(), 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	linkAttributes(*obj.getVBO(), 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
}

void VAO::bind() {
	glBindVertexArray(ID);

	for (Mesh obj : objects)
		obj.bind();
}

void VAO::draw() {
	for (Mesh obj : objects)
		glDrawElements(GL_TRIANGLES, obj.getISize(), GL_UNSIGNED_INT, 0);
}

void VAO::unbind() {
	for (Mesh obj : objects)
		obj.unbind();

	glBindVertexArray(0);
}

void VAO::destroy() {
	for (Mesh obj : objects)
		obj.destroy();

	glDeleteVertexArrays(1, &ID);
}

void VAO::setAlpha(float alpha) {
	this->alpha = alpha;
}

void VAO::clearBounds() {
	posiBinds.clear();
}

bool VAO::collidesWith(float x, float y, float z) {
	for (Mesh m : objects)
		if (m.collidesWith(x, y, z)) return true;

	return false;
}

bool VAO::collidesWith(vec3 pos) {
	return collidesWith(pos.r, pos.g, pos.b);
}

void VAO::bindMeshTextures() {
	for (Mesh obj : objects)
		obj.bindTextures();
}

void VAO::unbindMeshTextures() {
	for (Mesh obj : objects)
		obj.unbindTextures();
}

void VAO::registerMeshTextures(Shader& shader) {
	for (Mesh mesh : objects)
		mesh.registerTextures(shader);
}

void VAO::translate(float x, float y, float z) {
	translation.r = x;
	translation.g = y;
	translation.b = z;
}

void VAO::rotate(float w, float x, float y, float z) {
	rotation.w = w;
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void VAO::rotateEuler(float x, float y, float z) {
	vec3 euler(x, y, z);
	quat quaterion = quat(euler);

	orientation = euler;
	rotate(quaterion.w, quaterion.x, quaterion.y, quaterion.z);
}

void VAO::scale(float x, float y, float z) {
	scaling.r = x;
	scaling.g = y;
	scaling.b = z;
}

void VAO::bindPositionTo(DFloat x, DFloat y, DFloat z) {
	posiBinds.clear();
	posiBinds.push_back(x);
	posiBinds.push_back(y);
	posiBinds.push_back(z);
}

void VAO::bindRotationTo(DFloat x, DFloat y, DFloat z) {
	oriBinds.clear();
	oriBinds.push_back(x);
	oriBinds.push_back(y);
	oriBinds.push_back(z);
}
