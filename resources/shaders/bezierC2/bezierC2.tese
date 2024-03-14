#version 460 core

layout (isolines, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

patch in float patchColorek;
out float colorek;

vec3 deCasteljau(float t, vec3 coefficients[5]) {
    int n = coefficients.length();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < (n - i); j++) {
            coefficients[j] = coefficients[j] * (1 - t) + coefficients[j + 1] * t;
        }
    }
    return coefficients[0];
}

void
main()
{
    float t = gl_TessCoord.x;

    vec3 coefficients[5] = vec3[](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz, gl_in[4].gl_Position.xyz);
    vec4 pos = vec4( deCasteljau(t, coefficients), 1.f );
    gl_Position = projection * view * pos;
    colorek = patchColorek;
}