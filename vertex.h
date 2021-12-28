#ifndef VERTEX_H
#define VERTEX_H

#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace glm;

struct Vertex {
	vec3 coords;
	vec3 normal;
	vec3 color;
	vec2 texUV;

	Vertex(vec3 coords, vec3 color, vec2 texUV, vec3 normal) : coords(coords), color(color), texUV(texUV), normal(normal)
		{}
};
#endif