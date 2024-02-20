#version 460 core
out vec4 FragColor;

in float depth;


void main()
{
    FragColor = vec4(1-depth,1-depth,1-depth, 1.0f);
}