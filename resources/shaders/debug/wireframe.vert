#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
    float zDepth;
} vs_out;

uniform mat4 view;
uniform mat4 model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    float depth = (2.0 - (1.0 + (view * model * vec4(aPos, 1.0)).z))/2.0;
    vs_out.zDepth = log(depth);
    gl_Position = view * model * vec4(aPos, 1.0);
}