#version 330 core

out vec4 FragColor;

uniform vec4 lightColor;
uniform vec4 depthColor;

float linearizeDepth(float depth, float near = 0.1f, float far = 100.0f) {
	return (2.0f * near * far) / (far + near - (depth * 2.0f - 1.0f) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 0.5f) {
	return (1 / (1 + exp(-steepness * (linearizeDepth(depth) - offset))));
}

void main() {
	float depth = logisticDepth(gl_FragCoord.z, 0.2f, 0.5f);

	FragColor = lightColor * (1.0f - depth) + vec4(depth * vec3(depthColor.r, depthColor.g, depthColor.b), (depthColor.a * lightColor.a));
}