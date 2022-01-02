#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 translation;

void main() {
	currentPos = vec3(model * translation * vec4(aPos, 1.0f));

	gl_Position = camera * vec4(currentPos, 1.0);
}