#version 400 

layout (location = 0) in vec3 VertexPositions;

uniform mat4 modelMatrix;

void main()
{
    gl_Position = modelMatrix * vec4(VertexPositions, 1.0);
}