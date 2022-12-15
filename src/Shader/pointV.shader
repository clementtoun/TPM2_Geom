#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in uint aIsSelected;

flat out uint isSelected;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    isSelected = aIsSelected;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 7.0f;
}