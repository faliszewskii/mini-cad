#version 460 core

layout (isolines, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

uniform int division;
patch in int instanceID;

patch in float patchColorek;
out float colorek;

vec3 deBoor(float x, vec3 c[4]) {
    const int p = 3;

    for( int r = 1; r < p + 1 ; r++) {
        int m = p - r + 1;
        for(int j=p; j > r-1; j--) {
            int l = j - p + 1;
            float alpha = (x - l) / m;
            c[j] = (1.0 - alpha) * c[j - 1] + alpha * c[j];
        }
    }
    return vec3(c[p]);
}

vec3 seilerInterpolation(float t, vec3 coefficients[4]) {
    vec3 s1 = 3 * coefficients[1] - coefficients[0] - coefficients[3];
    vec3 s2 = 3 * coefficients[2] - coefficients[3] - coefficients[0];
    vec3 b03 = coefficients[0] * (1-t) + coefficients[3] * t;
    vec3 s12 = s1 * (1-t) + s2 * t;
    float d = t*(1-t);
    return b03 * (1-d) + s12 * d;
}

void
main()
{
    // Na wejściu mam
    float t = gl_TessCoord.x / division + float(instanceID) / division;
    float x = 1 + t; // x jest na środku

    vec3 coefficients[4] = vec3[](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);
    vec4 pos = vec4( deBoor(x, coefficients), 1.f );
    gl_Position = projection * view * pos;
    colorek = patchColorek;
}