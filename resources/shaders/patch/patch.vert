#version 460

layout(location = 0) in vec3 aPos;

out int instanceID_vert;
out int vertexID_vert;

void main(void)
{
    instanceID_vert = gl_InstanceID;
    gl_Position = vec4(aPos, 1);
}