#version 400

in vec3 normal;
in vec3 viewDirection;
in vec2 texCoords;

out vec4 fragmentColour;

uniform vec3 rimColour;
uniform float rimPower;

uniform sampler2D diffuse;

void main()
{
	vec3 N = normalize(normal);
	vec3 V = normalize(viewDirection);

	float rim = pow(1.0 - max(dot(N, V), 0.0), rimPower);

	vec4 baseColour = texture(diffuse, texCoords);

	fragmentColour = baseColour * vec4(0.2, 0.8, 0.2, 1.0);
	//fragmentColour = vec4(texCoords.x, texCoords.y, 0.0, 1.0);
}

