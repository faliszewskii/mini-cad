#version 460 core

layout (isolines, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

uniform int division;
patch in int instanceID;

uniform int fullCurves;
uniform int remainder;

patch in float patchColorek;
out float colorek;

vec3 deCasteljau(float t, vec3 coefficients[4], int n) {
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
    float t = gl_TessCoord.x / division + float(instanceID) / division;

    vec3 coefficients[4] = vec3[](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);
    int n = gl_PrimitiveID == fullCurves? remainder: coefficients.length();
    vec4 pos = vec4( deCasteljau(t, coefficients, n), 1.f );
    gl_Position = projection * view * pos;
    colorek = patchColorek;
}