#version 400

in vec2 texCoords;

out vec4 fragmentColour;

uniform bool hasTexture; //we use this as a selector to determine if collider is highlighted not for actually having a texture
uniform sampler2D diffuse;

void main()
{

	vec4 baseColour = texture(diffuse, texCoords);

	if (hasTexture)
		fragmentColour = baseColour * vec4(1.0, 0.0, 1.0, 1.0);
	else
		fragmentColour = baseColour * vec4(0.0, 1.0, 0.0, 1.0);

}

