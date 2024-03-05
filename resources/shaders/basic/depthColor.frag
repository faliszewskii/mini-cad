#version 460 core
out vec4 FragColor;

in float depth;
uniform bool selected;

void main()
{
    float I = 1 - depth;
    FragColor = selected ? vec4(1.0f, 0.5f, 0, 1.0f) :  vec4(I, I, I, 1.0f);
}