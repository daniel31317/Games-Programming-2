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

	fragmentColour = baseColour + vec4(rimColour, 1.0) * rim;
}

