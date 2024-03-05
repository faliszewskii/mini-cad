#version 460 core

uniform float time;
uniform vec4 color;

uniform vec3 beginPointPos;
uniform vec3 endPointPos;

uniform mat4 viewInv;
uniform mat4 projectionInv;

out vec4 FragColor;

in vec3 fragPos;

void main()
{
    const float segmentLenght = 0.01f;
    const float speed = 25;
    vec3 v = endPointPos - beginPointPos;
    vec4 worldSpacePos = viewInv * projectionInv * vec4(fragPos, 1);
    worldSpacePos /= worldSpacePos.w;

    float ratio = (worldSpacePos.xyz - beginPointPos).length() / v.length();
//    int segment = int((fragPos.y - sign(fragPos.y) * time/speed) / segmentLenght);
//    FragColor = segment%2==0 ? color : vec4(0);
    FragColor = vec4(ratio, 1, 0, 1);
}
