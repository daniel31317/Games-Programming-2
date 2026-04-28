#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normals;


uniform mat4 transform;


out vec2 texCoords;

void main()
{

	gl_Position = transform * vec4(VertexPosition, 1.0);

	texCoords = UV;
}
