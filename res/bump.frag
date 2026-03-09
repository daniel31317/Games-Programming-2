#version 430

in vec2 t_coords;

uniform sampler2D diffuse;
uniform sampler2D normalT;

out vec4 fragmentColor;

void main()
{
    vec3 normal = texture(normalT, t_coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 colour = texture2D(diffuse, t_coords).rgb;
    vec3 ambient = 0.8 * colour;

    vec3 lightDir = normalize(vec3(0.5, 0.5, 1.0));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseContrib = diff * colour;

    fragmentColor = vec4(ambient + diffuseContrib, 1.0);
}
