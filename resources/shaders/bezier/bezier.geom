#version 460 core
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 256) out;

uniform int segmentCount;

uniform mat4 projection;
uniform mat4 view;

vec3 deCasteljau(float t, vec3 coefficients[4]) {
    int n = coefficients.length();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < (n - i); j++) {
            coefficients[j] = coefficients[j] * (1 - t) + coefficients[j + 1] * t;
        }
    }
    return coefficients[0];
}

void main()
{
    float count = segmentCount <= 255 ? segmentCount : 255;
    float dt = 1.f / count; // TODO Maybe Chebyshev ???
    vec3 coefficients[4] = vec3[](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);

    for(int i = 0; i < count + 1; i++) {
        vec3 p = deCasteljau(i * dt, coefficients);
        gl_Position = projection * view * vec4(p, 1);
        EmitVertex();
    }
    EndPrimitive();
}