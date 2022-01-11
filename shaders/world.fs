#version 330 core

out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
in vec3 normalVec;
in vec3 currentPos;
in vec3 camPos;
in mat3 TBN;

uniform float lightColor[64];
uniform float lightType[64];
uniform float lightPos[64];
uniform float lightOrientation[64];

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform float vAlpha;

uniform vec4 depthColor;

float linearizeDepth(float depth, float near = 0.05f, float far = 100.0f) {
	return (2.0f * near * far) / (far + near - (depth * 2.0f - 1.0f) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.2f, float offset = 0.5f) {
	return (1 / (1 + exp(-steepness * (linearizeDepth(depth) - offset))));
}

vec4 sourceLight(vec3 light, vec4 light_color) {
	float a = 0.3f;
	float b = 0.02f;
	float specLight = 0.5f;

	vec3 lightV = light - currentPos;
	float dist = length(lightV);

	vec3 nrm = normalize(texture(tex2, texCoord).xyz * 2.0f - 1.0f);
	vec3 lightDirection = normalize(lightV);

	float diffuse = max(dot(nrm, lightDirection), 0.0f);
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specLight = 0.50f;
		vec3 viewDirection = normalize(camPos - currentPos);
		vec3 refDirection = reflect(-lightDirection, nrm);

		float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 32);
		specular = specAmount * specLight;
	};

	float intensity = 1.0f / (a * dist * dist+b * dist+1.0f);

	return (texture(tex0, texCoord) * (diffuse * intensity) + texture(tex1, texCoord).r * specular * intensity) * light_color;
}

vec4 spotLight(vec3 ori, vec3 light, vec4 light_color) {
	float outCone = 0.82f;
	float inCone = 0.95f;

	vec3 nrm = normalize(texture(tex2, texCoord).xyz * 2.0f - 1.0f);

	vec3 lightDirection = normalize(light - currentPos);

	float diffuse = max(dot(nrm, lightDirection), 0.0f);
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specLight = 0.70f;
		vec3 viewDirection = normalize(camPos - currentPos);
		vec3 reflectDirection = reflect(-lightDirection, nrm);
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);

		float specAmount = pow(max(dot(nrm, halfwayDirection), 0.0f), 32);
		specular = specAmount * specLight;
	};

	float angle = dot(ori, -lightDirection);
	float intensity = clamp((angle - outCone) / (inCone - outCone), 0.0f, 1.0f);

	return (texture(tex0, texCoord) * (diffuse * intensity) + texture(tex1, texCoord).r * specular * intensity) * light_color;
}

vec4 completeLight(vec4 light_color) {
	vec3 source = vec3(1.0f, 1.0f, 0.0f);

	float ambient = 0.15f;

	vec3 nrm = normalize(texture(tex2, texCoord).xyz * 2.0f - 1.0f);
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

	for(int i = 0; i < lightPos.length(); i+=3) {
		vec3 pos = TBN * vec3(lightPos[i], lightPos[i+1], lightPos[i+2]);
		vec3 ori = TBN * vec3(lightOrientation[i], lightOrientation[i+1], lightOrientation[i+2]);
		vec4 col = vec4(lightColor[i], lightColor[i+1], lightColor[i+2], 1.0f);

		if(lightType[ty] == 0.0f)
			output += spotLight(ori, pos, col);
		else if(lightType[ty] == 1.0f)
			output += sourceLight(pos, col);
		else if(lightType[ty] == 2.0f)
			output += completeLight(col);

		ty++;
	}

	float depthTexture = logisticDepth(gl_FragCoord.z);

	FragColor = output * (1 - depthTexture) + vec4(depthTexture * vec3(depthColor.r, depthColor.g, depthColor.b), 1.0f);
}