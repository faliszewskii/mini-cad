#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 position;

out vec3 color;

void addAxis(vec3 axis, vec3 axisColor) {
    const float length = 0.1f;
    gl_Position = projection * view * vec4(position, 1);
    color = axisColor;
    EmitVertex();
    gl_Position = projection * view * vec4(position + axis * length, 1);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    addAxis(vec3(1,0,0), vec3(1, 0.4, 0.4));
    addAxis(vec3(0,1,0), vec3(0.4, 1, 0.4));
    addAxis(vec3(0,0,1), vec3(0.4, 0.4, 1));
}