#version 460 core
out vec4 FragColor;

uniform bool selected;
uniform vec4 color;

uniform sampler2D mask;

in vec2 texCoords;

void main()
{
    vec4 v = texture(mask, texCoords);
    if(v.x != 0 && v.y != 0 && v.z != 0) discard;

    vec4 c = {color.r+0.0f, color.g, color.b+0.1f, color.a};
    FragColor = selected ? mix(vec4(0.5f, 0.4f, 0, c.a) , c, 0.2): c;
}