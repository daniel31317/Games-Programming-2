#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normals;

uniform mat4 transform;

out vec4 v_pos; 
out vec2 t_coords; 

void main() 
{
	vec4 worldPosition = transform * vec4(VertexPosition, 1.0);
    
    v_pos = worldPosition;

    t_coords = UV;
    
    gl_Position = worldPosition;
}
