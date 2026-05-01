#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normals;
layout (location = 3) in vec3 VertexColour;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;
out vec3 vertexColour;
out vec3 objPos;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = modelMatrix * vec4(VertexPosition, 1.0);
    fragPos = vec3(worldPos);

    normal = normalize(mat3(transpose(inverse(modelMatrix))) * Normals);

    texCoords = UV;
    vertexColour = VertexColour;

    objPos = VertexPosition;

    gl_Position = projection * view * worldPos;
}