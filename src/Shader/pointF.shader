#version 330 core

flat in uint isSelected;

out vec4 FragColor;

uniform bool pointMode;

void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0) * isSelected;
}
