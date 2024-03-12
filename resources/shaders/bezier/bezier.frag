#version 460 core

out vec4 FragColor;
in float colorek; // TODO DEBUG

void main()
{
    FragColor = vec4(colorek,1-colorek,0,1);
}
