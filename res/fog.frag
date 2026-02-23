#version 430

out vec4 FragColor;

in vec4 v_pos; 

layout (location = 1) uniform vec3 fogColor;

float maxDist = 10.0f; //fog max distance
float minDist = 0.0f; //fog min distance

void main() 
{
    float dist = length(v_pos); 
    
    float fogFactor = (maxDist - dist) / (maxDist - minDist);

    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 objectColor = vec3(255, 0, 212) / 255.0;

    vec3 finalColor = mix(fogColor, objectColor, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}
