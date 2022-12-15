#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in uint aIsSelected;
layout (location = 3) in float af;

flat out uint out_isSelected;
out vec3 out_FragPos;
out vec3 out_Normal;
out float out_f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    out_isSelected = aIsSelected;
    out_FragPos = vec3(model * vec4(aPos, 1.0));
    out_Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(out_FragPos, 1.0);
    out_f = af;
}