#version 400

in vec3 normal;


void main()
{
	vec3 lightDir = vec3(0.0, -1.0, 0.5);

	float brightness = 1.0f;

    float intensity = dot(normal, lightDir) * brightness;

    vec3 baseColour = vec3(255, 0, 212) / 255.0;

    gl_FragColor = vec4(baseColour * intensity, 1.0);
}

