#version 460 core

layout (isolines, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

patch in float patchColorek;
out float colorek;

in float tc_knot[];

vec3 deBoor(float x, vec3 c[4], float t[7], int k) {
    const int p = 3;

    for( int r = 1; r < p + 1 ; r++) {
        for(int j=p; j > r-1; j--) {
            float alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p]);
            c[j] = (1.0 - alpha) * c[j - 1] + alpha * c[j];
        }
    }
    return vec3(c[p]);
}

void
main()
{
    // Na wejściu mam
    float t = gl_TessCoord.x; // 0 -> 1
    float x = tc_knot[3] + t;

    float knots[7] = {tc_knot[0], tc_knot[1], tc_knot[2], tc_knot[3], tc_knot[4], tc_knot[5], tc_knot[6]};

    vec3 coefficients[4] = vec3[](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);
    vec4 pos = vec4( deBoor(x, coefficients, knots, 3), 1.f );
    gl_Position = projection * view * pos;
    colorek = patchColorek;
}
/*
Arguments
    ---------
    k: Index of knot interval that contains x.
    x: Position.
    t: Array of knot positions, needs to be padded as described above.
    c: Array of control points.
    p: Degree of B-spline.
    """
    d = [c[j + k - p] for j in range(0, p + 1)]

    for r in range(1, p + 1):
        for j in range(p, r - 1, -1):
            alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p])
            d[j] = (1.0 - alpha) * d[j - 1] + alpha * d[j]

    return d[p]
*/