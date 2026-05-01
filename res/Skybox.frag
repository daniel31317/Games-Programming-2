#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

// samplerCube is the special uniform type for Cubemaps
uniform samplerCube skybox;

void main()
{    
    // We sample the 6-sided texture using the 3D coordinate TexCoords
    FragColor = texture(skybox, TexCoords);
}