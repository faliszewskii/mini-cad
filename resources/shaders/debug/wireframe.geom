#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 projection;

void main()
{
    gl_Position = projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projection * gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = projection * gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}