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
uniform sampler2D tex3;

uniform float vAlpha;

uniform vec4 depthColor;

float linearizeDepth(float depth, float near = 0.05f, float far = 100.0f) {
	return (2.0f * near * far) / (far + near - (depth * 2.0f - 1.0f) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.2f, float offset = 0.5f) {
	return (1 / (1 + exp(-steepness * (linearizeDepth(depth) - offset))));
}

vec2 applyParallaxOcclusion(vec3 viewD) {
	float heightScale = 0.05f;
	const float minLayers = 8.0f;
    const float maxLayers = 16.0f;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewD)));
	float layerDepth = 1.0f / numLayers;
	float currentLayerDepth = 0.0f;
	
	// Increase the "holes" aberration
	vec2 S = viewD.xy / viewD.z * heightScale; 
    vec2 deltaUVs = S / numLayers;
	
	vec2 UVs = texCoord;
	float currentDepthMapValue = 1.0f - texture(tex3, UVs).r;
	
	// Loop till the point on the heightmap is "hit"
	while(currentLayerDepth < currentDepthMapValue) {
        UVs -= deltaUVs;
        currentDepthMapValue = 1.0f - texture(tex3, UVs).r;
        currentLayerDepth += layerDepth;
    }

	// Interpolates with previous value
	vec2 prevTexCoords = UVs + deltaUVs;
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1.0f - texture(tex3, prevTexCoords).r - currentLayerDepth + layerDepth;
	float weight = afterDepth / (afterDepth - beforeDepth);
	UVs = prevTexCoords * weight + UVs * (1.0f - weight);

	return UVs;
}

vec2 applyDiffuseSpecular(vec3 norm, vec3 lightD, vec3 viewD, bool spot = false) {
	vec2 diffspec;

	float diffuse = max(dot(norm, lightD), 0.0f);
	float specular = 0.0f;
	if (diffuse != 0.0f) {
		float specLight = 0.70f;
		vec3 phong = reflect(-lightD, norm);
		vec3 blinnPhong = normalize(lightD + viewD);

		float specAmount;
		if(spot) 
			specAmount = pow(max(dot(norm, blinnPhong), 0.0f), 32);
		else 
			specAmount = pow(max(dot(viewD, blinnPhong), 0.0f), 32);

		specular = specAmount * specLight;
	};

	diffspec.r = diffuse;
	diffspec.g = specular;

	return diffspec;
}


vec4 sourceLight(vec3 light, vec4 light_color) {
	float a = 0.3f;
	float b = 0.02f;
	float specLight = 0.5f;

	vec3 lightV = light - currentPos;
	float dist = length(lightV);
	vec3 viewDirection = normalize(camPos - currentPos);

	vec2 UVs = applyParallaxOcclusion(viewDirection);

	vec3 nrm = normalize(texture(tex2, UVs).xyz * 2.0f - 1.0f);
	vec3 lightDirection = normalize(lightV);

	vec2 diffspec = applyDiffuseSpecular(nrm, lightDirection, viewDirection);
	float diffuse = diffspec.r;
	float specular = diffspec.g;

	float intensity = 1.0f / (a * dist * dist+b * dist+1.0f);

	return (texture(tex0, UVs) * (diffuse * intensity) + texture(tex1, UVs).r * specular * intensity) * light_color;
}

vec4 spotLight(vec3 ori, vec3 light, vec4 light_color) {
	float outCone = 0.77f;
	float inCone = 0.95f;

	vec3 lightV = light - currentPos;
	vec3 viewDirection = normalize(camPos - currentPos);

	vec2 UVs = applyParallaxOcclusion(viewDirection);

	vec3 nrm = normalize(texture(tex2, UVs).xyz * 2.0f - 1.0f);
	vec3 lightDirection = normalize(lightV);

	vec2 diffspec = applyDiffuseSpecular(nrm, lightDirection, viewDirection, true);
	float diffuse = diffspec.r;
	float specular = diffspec.g;

	float angle = dot(ori, -lightDirection);
	float intensity = clamp((angle - outCone) / (inCone - outCone), 0.0f, 1.0f);

	return (texture(tex0, UVs) * (diffuse * intensity) + texture(tex1, UVs).r * specular * intensity) * light_color;
}

vec4 completeLight(vec4 light_color) {
	vec3 source = vec3(1.0f, 1.0f, 0.0f);
	float ambient = 0.15f;

	vec3 viewDirection = normalize(camPos - currentPos);

	vec2 UVs = applyParallaxOcclusion(viewDirection);

	vec3 nrm = normalize(texture(tex2, texCoord).xyz * 2.0f - 1.0f);
	vec3 lightDirection = normalize(source);

	float diffusion = max(dot(nrm, lightDirection), 0.0f);
	
	vec3 blinnPhong = normalize(lightDirection + viewDirection);

	float specAmount = pow(max(dot(viewDirection, blinnPhong), 0.0f), 32);
	float specular = specAmount * 0.70f;

	return (texture(tex0, UVs) * (diffusion + ambient) + texture(tex1, UVs).r * specular) * light_color;
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