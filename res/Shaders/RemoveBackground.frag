#version 430

out vec4 FragColor;

in vec2 uv;

uniform sampler2D diffuse;

void main() {
    vec4 color = texture(diffuse, uv);
    
    if (color.a == 0.0) {
        discard;
    }
    
    FragColor = color;
}