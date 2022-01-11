#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 color;
out vec2 texCoord;
out vec3 normalVec;
out vec3 currentPos;
out vec3 camPos;

out mat3 TBN;

in DATA {
	vec3 color;
	vec2 texCoord;
	vec3 normalVec;

	mat4 model;
	vec3 camPos;

	mat4 camera;
} data_in[];

void processPoint(int x, mat3 TBN) {
	gl_Position = data_in[x].camera * gl_in[x].gl_Position;
	color = data_in[x].color;
	texCoord = data_in[x].texCoord;
	normalVec = data_in[x].normalVec;
	currentPos = TBN * gl_in[x].gl_Position.xyz;
	camPos = TBN * data_in[x].camPos;

	EmitVertex();
}

void main() {
	// TBN Matrix calculation
	// Inspired by Victor Gordan's video
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;

	float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(data_in[0].model * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(data_in[0].model * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(data_in[0].model * vec4(cross(edge1, edge0), 0.0f)));

    TBN = mat3(T, B, N);
    TBN = transpose(TBN);

	
	// Points
	processPoint(0, TBN);
	processPoint(1, TBN);
	processPoint(2, TBN);

	EndPrimitive();
}