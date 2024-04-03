#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in float knot;

out int instanceID_vert;

void main(void)
{
    instanceID_vert = gl_InstanceID;
    gl_Position = vec4(aPos, 1);
}