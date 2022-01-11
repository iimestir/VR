#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 normalVec;
out vec3 currentPos;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main() {
	currentPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));

	gl_Position = camera * vec4(currentPos, 1.0);
	color = aColor;
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	normalVec = aNormal;
}