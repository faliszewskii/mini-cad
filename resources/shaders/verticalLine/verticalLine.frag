#version 460 core

uniform float time;
uniform vec4 color;

uniform vec3 beginPointPos;
uniform vec3 endPointPos;

out vec4 FragColor;

in vec3 fragPos;

void main()
{
    const float segmentLength = 0.01f;
    const float speed = 2.5;

    int isSegment = int(length(fragPos - beginPointPos)  / segmentLength - time * speed);
    FragColor = isSegment % 2 == 0 ? vec4(color) : vec4(0);
}
