#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform vec2 resolution;
uniform int ppType;

vec4 effectBW() {
	vec4 tex = texture(screenTexture, texCoords);
	float avg = (tex.x, tex.y, tex.z);

	return vec4(avg, avg, avg, 1.0f);
}

vec4 effectInvert() {
	return vec4(1.0f) - texture(screenTexture, texCoords);
}

vec4 noEffect() {
	return texture(screenTexture, texCoords);
}

vec4 effectBorderDetection() {
	float offset_x = 1.0f / resolution.x;
	float offset_y = 1.0f / resolution.y;

	// offset from the pixel we are on compared to its neighbours
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset_x, offset_y), vec2(0.0f, offset_y), vec2(offset_x, offset_y),
		vec2(-offset_x, 0.0f), vec2(0.0f, 0.0f), vec2(offset_x, 0.0f),
		vec2(-offset_x, -offset_y), vec2( 0.0f, -offset_y), vec2( offset_x, -offset_y) 
	);

	// matrix defining how important each pixel is, <1 : darker ; >1 : brighter
	float kernel[9] = float[]
	(
		1,1,1,
		1,-8,1,
		1,1,1
	);

	vec3 color = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];

    return vec4(color, 1.0f);
}

vec4 effectOutline() {
	float offset_x = 1.0f / resolution.x;
	float offset_y = 1.0f / resolution.y;

	// offset from the pixel we are on compared to its neighbours
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset_x, offset_y), vec2(0.0f, offset_y), vec2(offset_x, offset_y),
		vec2(-offset_x, 0.0f), vec2(0.0f, 0.0f), vec2(offset_x, 0.0f),
		vec2(-offset_x, -offset_y), vec2( 0.0f, -offset_y), vec2( offset_x, -offset_y) 
	);

	float kernel[9] = float[] (
		0,-1,0,
		-1,5,-1,
		0,-1,0
	);

	vec3 color = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];

    return vec4(color, 1.0f);
}

void main() {
	vec4 output = vec4(0.0f);

	switch(ppType) {
		case 0:
			output = noEffect();
			break;
		case 1:
			output = effectInvert();
			break;
		case 2:
			output = effectBW();
			break;
		case 3:
			output = effectBorderDetection();
			break;
		case 4:
			output = effectOutline();
			break;
		default:
			output = noEffect();
	}

	FragColor = output;
}