#version 460 core

uniform float time;

out vec4 FragColor;

in vec3 fragPos;

void main()
{
    const float segmentLenght = 0.01f;
    const float speed = 25;
    int segment = int((fragPos.y - sign(fragPos.y) * time/speed) / segmentLenght);
    FragColor = segment%2==0 ? vec4(1.0, 1.0, 1.0, .5): vec4(0);
}
