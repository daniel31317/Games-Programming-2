#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    // We remove translation from the view matrix in the C++ struct,
    // but we still multiply by projection to get the right perspective.
    vec4 pos = projection * view * vec4(aPos, 1.0);
    
    // Use .xyww trick to ensure depth is always 1.0
    // This forces the skybox to be behind everything else
    gl_Position = pos.xyww;
}