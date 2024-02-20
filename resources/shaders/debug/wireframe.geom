#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in VS_OUT {
    float zDepth;
} gs_in[];

out float depth;
uniform mat4 projection;

void main()
{
    gl_Position = projection * gl_in[0].gl_Position;
    depth = gs_in[0].zDepth;
    EmitVertex();
    gl_Position = projection * gl_in[1].gl_Position;
    depth = gs_in[1].zDepth;
    EmitVertex();
    gl_Position = projection * gl_in[2].gl_Position;
    depth = gs_in[2].zDepth;
    EmitVertex();
    EndPrimitive();
}