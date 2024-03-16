#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in float knot;

out float v_knot;

void main(void)
{
    gl_Position = vec4(aPos, 1);
    v_knot = knot;
}