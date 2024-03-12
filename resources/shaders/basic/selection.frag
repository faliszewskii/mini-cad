#version 460 core
out vec4 FragColor;

uniform bool selected;

void main()
{
    FragColor = selected ? vec4(0.5f, 0.4f, 0, 1.0f) : vec4(0.0f, 0.0f, 0.0f, 1.0f);
}