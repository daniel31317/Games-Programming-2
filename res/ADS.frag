#version 400

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3 lightPos; 
uniform vec3 lightColor;   
uniform vec3 objectColor;  
uniform vec3 camPos;
uniform sampler2D diffuse;

out vec4 fragmentColor;

float ambientStrength = 0.4;
float specularStrength = 0.8;
float shininess = 32.0;     

void main()
{
    vec3 texColor = texture(diffuse, texCoords).rgb;

    //ambient
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular (phong)
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = (ambient + diffuse + specular);
    vec3 result = lighting * texColor * objectColor;

    fragmentColor = vec4(result, 1.0);
}

