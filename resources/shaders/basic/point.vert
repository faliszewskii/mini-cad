#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out float depth;

uniform vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat4 buildTranslation(vec3 delta)
{
    return mat4(
    vec4(1.0, 0.0, 0.0, 0.0),
    vec4(0.0, 1.0, 0.0, 0.0),
    vec4(0.0, 0.0, 1.0, 0.0),
    vec4(delta, 1.0));
}

void main()
{
    vec4 coord = projection * view * buildTranslation(position) * model * vec4(aPos, 1.0);
    depth = coord.z;
    gl_Position = coord;
}