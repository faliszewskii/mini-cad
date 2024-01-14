#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    float shininess;
    vec4 albedo;
    bool useAlbedoTexture;
    sampler2D textureAlbedo;
};
uniform Material material;

void main()
{
    if(material.useAlbedoTexture)
        FragColor = texture(material.textureAlbedo, TexCoords);
    else
        FragColor = material.albedo;
}