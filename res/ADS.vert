#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normals;
layout (location = 3) in vec3 VertexColour; 


uniform mat4 transform;
uniform mat4 modelMatrix;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;
out vec3 vertexColour;  

void main()
{
	vec4 worldPos = modelMatrix * vec4(VertexPosition, 1.0);

    fragPos = vec3(worldPos);

    normal = normalize(mat3(transpose(inverse(modelMatrix))) * Normals);

    gl_Position = transform * vec4(VertexPosition, 1.0);

    texCoords = UV;

    vertexColour = VertexColour;
}
