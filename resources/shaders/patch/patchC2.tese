#version 460 core

layout (isolines, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

uniform int gridCountWidth;
uniform int gridCountLength;
uniform int patchCountWidth;
uniform int patchCountLength;

patch in int instanceID;

out float outU;
out float outV;

vec4 descendingAlgorithm(float t, int n)
{
    float N01 = 1;

    float N10 = N01 * (1 - t);
    float N11 = N01 * (t - 0);

    float N2_1 = N10 * (1 - t)/2.f;
    float N20 = N10 * (t - (-1))/2.f + N11 * (2 - t)/2.f;
    float N21 = N11 * (t - 0)/2.f;

    float N3_2 = N2_1 * (1 - t)/3.f;
    float N3_1 = N2_1 * (t - (-2))/3.f + N20 * (2 - t)/3.f;
    float N30 = N20 * (t - (-1))/3.f + N21 * (3 - t)/3.f;
    float N31 = N21 * (t - 0)/3.f;

    return vec4(N3_2, N3_1, N30, N31);
}


vec3 deCasteljau2D(vec3[4][4] coefficients, float u, float v)
{
    vec4 uCoeffs = descendingAlgorithm(u, 3);
    vec4 vCoeffs = descendingAlgorithm(v, 3);

    vec3 result = vec3(0.0, 0.0, 0.0);

    for (int u = 0; u < 4; u++)
    {
        vec3 partResult = vec3(0.0, 0.0, 0.0);

        partResult += coefficients[u][0] * vCoeffs[0];
        partResult += coefficients[u][1] * vCoeffs[1];
        partResult += coefficients[u][2] * vCoeffs[2];
        partResult += coefficients[u][3] * vCoeffs[3];

        result += partResult * uCoeffs[u];
    }

    return result;
}

void main()
{
    bool b = instanceID == 0;

    float x = gl_TessCoord.x;
    float y = gl_TessCoord.y;

    float u = b ? x : y * (gridCountWidth+1.f)/(gridCountWidth);
    float v = b ? y * (gridCountLength+1.f)/(gridCountLength) : x ;

    vec3[4][4] coefficients = {
    vec3[4](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz),
    vec3[4](gl_in[4].gl_Position.xyz, gl_in[5].gl_Position.xyz, gl_in[6].gl_Position.xyz, gl_in[7].gl_Position.xyz),
    vec3[4](gl_in[8].gl_Position.xyz, gl_in[9].gl_Position.xyz, gl_in[10].gl_Position.xyz, gl_in[11].gl_Position.xyz),
    vec3[4](gl_in[12].gl_Position.xyz, gl_in[13].gl_Position.xyz, gl_in[14].gl_Position.xyz, gl_in[15].gl_Position.xyz)
    };

    vec4 pos = vec4(deCasteljau2D(coefficients, u, v), 1.f);
    gl_Position = projection * view * pos;
    float uPatch = gl_PrimitiveID / patchCountWidth;
    float vPatch = gl_PrimitiveID % patchCountWidth;
    outU = (u + uPatch) / patchCountLength;
    outV = (v + vPatch) / patchCountWidth;
}