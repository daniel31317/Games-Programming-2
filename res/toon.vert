#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 Normals;

out vec3 normal;
uniform mat4 transform;


void main()
{
	normal = Normals;
	gl_Position = transform * vec4(VertexPosition, 1.0);
}
