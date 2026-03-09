#version 430

out vec4 FragColor;

in vec4 v_pos; 

in vec2 t_coords; 

uniform vec3 fogColor;

uniform sampler2D diffuse;

float maxDist = 8.0f; //fog max distance
float minDist = 0.0f; //fog min distance

void main() 
{
    float dist = length(v_pos); 
    
    float fogFactor = (maxDist - dist) / (maxDist - minDist);

    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec4 texColor = texture(diffuse, t_coords);

    vec3 finalColor = mix(fogColor, texColor.rgb, fogFactor);

    FragColor = vec4(finalColor, texColor.a);
}
