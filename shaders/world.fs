#version 330 core

out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 camPos;
uniform vec3 lightOrientation;

uniform int lightSize;
uniform float lightPos[256];
uniform float lightColor[256];
uniform float lightType[256];
uniform float vAlpha;

uniform vec4 depthColor;

float linearizeDepth(float depth, float near = 0.1f, float far = 100.0f) {
	return (2.0f * near * far) / (far + near - (depth * 2.0f - 1.0f) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 1.0f) {
	return (1 / (1 + exp(-steepness * (linearizeDepth(depth) - offset))));
}

vec4 pointLight(vec3 light, vec4 light_color) {
	float a = 0.05;
	float b = 0.02;
	float ambient = 0.15f;
	float specLight = 0.50f;

	vec3 lightV = light - currentPos;
	float dist = length(lightV);

	float intensity = 1.0f / (a * dist * dist+b * dist+1.0f);

	vec3 nrm = normalize(normal);
	vec3 lightDirection = normalize(lightV);

	float diffusion = max(dot(nrm, lightDirection), 0.0f);
	
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 refDirection = reflect(-lightDirection, nrm);

	float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	return (texture(tex0, texCoord) * (diffusion * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * light_color;
}

vec4 spotLight(vec3 ori, vec3 light, vec4 light_color) {
	float outCone = 0.90f;
	float inCone = 0.95f;
	float ambient = 0.15f;

	vec3 nrm = normalize(normal);
	vec3 lightDirection = normalize(light - currentPos);

	float diffusion = max(dot(nrm, lightDirection), 0.0f);

	float specLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 refDirection = reflect(-lightDirection, nrm);

	float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	float angle = dot(ori, -lightDirection);
	float intensity = clamp((angle - outCone) / (inCone - outCone), 0.0f, 1.0f);

	return (texture(tex0, texCoord) * (diffusion * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * light_color;
}

vec4 directLight(vec4 light_color) {
	vec3 source = vec3(1.0f, 1.0f, 0.0f);

	float ambient = 0.15f;

	vec3 nrm = normalize(normal);
	vec3 lightDirection = normalize(source);

	float diffusion = max(dot(nrm, lightDirection), 0.0f);

	float specLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 refDirection = reflect(-lightDirection, nrm);

	float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	return (texture(tex0, texCoord) * (diffusion + ambient) + texture(tex1, texCoord).r * specular) * light_color;
}

void main() {
	vec4 output = vec4(0.0f);

	int ty = 0;

	vec3 ori = vec3(0.85f, -1.0f, 0.0f);

	for(int i = 0; i < lightSize*3; i+=3) {
		vec3 pos = vec3(lightPos[i], lightPos[i+1], lightPos[i+2]);

		if(lightType[ty] == 0.0f)
			output += spotLight(lightOrientation, pos, vec4(vec4(lightColor[i], lightColor[i+1], lightColor[i+2], 1.0f)));
		else if(lightType[ty] == 1.0f)
			output += pointLight(pos, vec4(vec4(lightColor[i], lightColor[i+1], lightColor[i+2], 1.0f)));
		else if(lightType[ty] == 2.0f)
			output += directLight(vec4(vec4(lightColor[i], lightColor[i+1], lightColor[i+2], 1.0f)));

		ty++;
	}

	float depthTexture = logisticDepth(gl_FragCoord.z);

	FragColor = output * (1.0f - depthTexture) + vec4(depthTexture * vec3(depthColor.r, depthColor.g, depthColor.b), 1.0f);
}