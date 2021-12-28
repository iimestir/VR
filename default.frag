#version 330 core

out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight() {
	vec3 lightV = lightPos - currentPos;
	float dist = length(lightV);
	float a = 1.0;
	float b = 0.04;
	float intensity = 1.0f / (a * dist * dist+b * dist+1.0f);

	float ambient = 0.15f;

	vec3 nrm = normalize(normal);
	vec3 lightDirection = normalize(lightV);

	float diffusion = max(dot(nrm, lightDirection), 0.0f);

	float specLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 refDirection = reflect(-lightDirection, nrm);

	float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	return (texture(tex0, texCoord) * (diffusion * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

vec4 directLight() {
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

	return (texture(tex0, texCoord) * (diffusion + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
	float outCone = 0.90f;
	float inCone = 0.95f;

	float ambient = 0.15f;

	vec3 nrm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diffusion = max(dot(nrm, lightDirection), 0.0f);

	float specLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 refDirection = reflect(-lightDirection, nrm);

	float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outCone) / (inCone - outCone), 0.0f, 1.0f);

	return (texture(tex0, texCoord) * (diffusion + ambient + intensity) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

void main() {
	FragColor = pointLight();
}