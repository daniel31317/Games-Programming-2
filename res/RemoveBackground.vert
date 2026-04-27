#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;

out vec2 uv;

uniform mat4 transform;

void main() {
    uv = UV;
    gl_Position = transform * vec4(VertexPosition, 1.0);
}