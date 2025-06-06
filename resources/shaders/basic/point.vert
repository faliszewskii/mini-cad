#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out float depth;

uniform vec3 position;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 coord = projection * view * vec4(aPos + position, 1.0);
    depth = coord.z;
    gl_Position = coord;
}