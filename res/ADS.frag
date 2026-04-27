#version 400

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;
in vec3 vertexColour;

uniform vec3 lightPos; 
uniform vec3 lightColour;   
uniform vec3 camPos;
uniform sampler2D diffuse;

out vec4 fragmentColour;

float ambientStrength = 0.4;
float specularStrength = 0.8;
float shininess = 32.0;     

void main()
{
    vec3 texColour = texture(diffuse, texCoords).rgb;

    //ambient
    vec3 ambient = ambientStrength * lightColour;

    //diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;

    //specular (phong)
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColour;

    vec3 lighting = (ambient + diffuse + specular);
    vec3 result = lighting * vertexColour;

    fragmentColour = vec4(result, 1.0);
}

