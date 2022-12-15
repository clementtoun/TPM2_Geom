#version 330 core
out vec4 FragColor;

flat in uint isSelected;
in vec3 FragPos;
in vec3 Normal;
in vec3 dist;
in float f;

uniform vec3 viewPos;
uniform bool drawline;
uniform bool f_mode;

vec3 CalcPointLight(vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewPos, vec3 color)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

    vec3 specular = vec3(0.3f) * spec; // assuming bright white light color

    return diffuse + specular;
}

void main()
{
    if(!f_mode) {
        vec3 color = vec3(0.6f, 0.6f, 0.6f);
        vec3 ambient = 0.05f * color;
        vec3 lightPos[3];
        lightPos[0] = vec3(0.0f, 75.0f, 100.0f);
        lightPos[1] = vec3(-25.0f, -50.0f, -100.0f);
        lightPos[2] = vec3(75.0f, 75.0f, -50.0f);
        vec3 res = vec3(0.0f, 0.0f, 0.0f);

        for(int i=0; i < 3; i++) {
            res += ambient + CalcPointLight(lightPos[i], normalize(Normal), FragPos, viewPos, color);
        }

        float d = min(dist[0], min(dist[1], dist[2]));
        float I = exp2(-1.5f*d*d) * uint(drawline);

        FragColor = vec4(I * vec3(0.1f, 0.f, 0.1f) * (uint(1)-isSelected) + (1.0f-I) * res, 1.0f);
    }
    else {
        FragColor = vec4(vec3(1.f, 0.f, 0.f) * f, 1.0f);
    }
}