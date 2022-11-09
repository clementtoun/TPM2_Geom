#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

vec3 CalcPointLight(vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewPos, vec3 color)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color

    return diffuse + specular;
}

void main()
{
    vec3 color = vec3(0.9f, 0.5f, 0.1f);
    vec3 ambient = 0.05 * color;
    vec3 lightPos[3];
    lightPos[0] = vec3(0.0f, 100.0f, 0.0f);
    lightPos[1] = vec3(-25.0f, -50.0f, -100.0f);
    lightPos[2] = vec3(75.0f, 75.0f, 0.0f);
    vec3 res = vec3(0.0f, 0.0f, 0.0f);

    for(int i=0; i < 3; i++) {
        res += ambient + CalcPointLight(lightPos[i], normalize(Normal), FragPos, viewPos, color);
    }

    FragColor = vec4(res, 1.0);
}