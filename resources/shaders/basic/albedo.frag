#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 color;

uniform sampler2D texture_diffuse;
uniform bool useTexture;

void main()
{
    if(useTexture)
        FragColor = texture(texture_diffuse, TexCoords);
    else
        FragColor = color;
}