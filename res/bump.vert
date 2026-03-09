#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextCoords;

uniform mat4 transform;

out vec2 t_coords; 

void main() 
{
    t_coords = TextCoords;

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
