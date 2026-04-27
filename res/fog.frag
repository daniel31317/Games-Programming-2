#version 430

out vec4 FragColour;

in vec4 v_pos; 

in vec2 t_coords; 

uniform vec3 fogColour;

uniform sampler2D diffuse;

float maxDist = 8.0f; //fog max distance
float minDist = 0.0f; //fog min distance

void main() 
{
    float dist = length(v_pos); 
    
    float fogFactor = (maxDist - dist) / (maxDist - minDist);

    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec4 texColour = texture(diffuse, t_coords);

    vec3 finalColour = mix(fogColour, texColour.rgb, fogFactor);

    FragColour = vec4(finalColour, texColour.a);
}
