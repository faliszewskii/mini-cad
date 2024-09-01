#version 460 core

uniform bool selected;
uniform vec4 color;

out vec4 FragColor;

uniform sampler2D mask;

in float outU;
in float outV;

void main()
{
    vec2 tex = vec2(outU, outV);
    vec4 v = texture(mask, tex);
    if(v.x != 0 && v.y != 0 && v.z != 0) discard;

   vec4 c = {color.r+0.0f, color.g, color.b+0.1f, color.a};
   FragColor = selected ? mix(vec4(0.5f, 0.4f, 0, c.a) , c, 0.2): c;
//    FragColor = vec4(outU, outV, 0, 1);
}
