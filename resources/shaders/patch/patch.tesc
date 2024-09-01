#version 460 core

layout (vertices = 16) out;

uniform mat4 projection;
uniform mat4 view;

uniform int gridCountWidth;
uniform int gridCountLength;

in int instanceID_vert[32];
patch out int instanceID;

in int vertexID_vert[32];
patch out int vertexID;

void main()
{
    instanceID = instanceID_vert[gl_InvocationID];
    bool b = instanceID == 0;


    gl_TessLevelOuter[0] = (b ? gridCountLength : gridCountWidth) + 1;
    gl_TessLevelOuter[1] = 400;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}