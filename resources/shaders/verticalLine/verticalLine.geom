#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 beginPointPos;
uniform vec3 endPointPos;

out vec3 fragPos;

void main()
{
    gl_Position = projection * view * vec4(beginPointPos, 1);
    fragPos = beginPointPos;
    EmitVertex();
    gl_Position = projection * view * vec4(endPointPos, 1);
    fragPos = endPointPos;
    EmitVertex();
    EndPrimitive();
}