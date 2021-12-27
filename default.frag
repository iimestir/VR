#version 330 core

out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


void main()
{
	float ambient = 0.15f;

	vec3 nrm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diffusion = max(dot(normal, lightDirection), 0.0f);

	float specLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 refDirection = reflect(-lightDirection, normal);

	float specAmount = pow(max(dot(viewDirection, refDirection), 0.0f), 8);
	float specular = specAmount * specLight;

	FragColor = texture(tex0, texCoord) * lightColor * (ambient + specular + diffusion);
}