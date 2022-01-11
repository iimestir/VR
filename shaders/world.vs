#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out DATA {
	vec3 color;
	vec2 texCoord;
	vec3 normalVec;

	mat4 model;
	vec3 camPos;

	mat4 camera;
} data_out;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

uniform vec3 camPos;

void main() {
	gl_Position = model * translation * rotation * scale * vec4(aPos, 1.0f);
	data_out.normalVec = aNormal;
	data_out.color = aColor;
	data_out.texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	data_out.model = model * translation * rotation * scale;
	data_out.camera = camera;
	data_out.camPos = camPos;
}