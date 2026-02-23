#version 430

layout (location = 0) in vec3 VertexPosition;

uniform mat4 transform;

out vec4 v_pos; 

void main() 
{
	vec4 worldPosition = transform * vec4(VertexPosition, 1.0);
    
    v_pos = worldPosition;
    
    gl_Position = worldPosition;
}
