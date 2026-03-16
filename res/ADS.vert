#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normals;


uniform mat4 transform;
uniform mat4 modelMatrix;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main()
{
	vec4 worldPos = modelMatrix * vec4(VertexPosition, 1.0);

    fragPos = vec3(worldPos);

    normal = normalize(mat3(modelMatrix) * Normals);

    gl_Position = transform * vec4(VertexPosition, 1.0);

    texCoords = UV;
}
