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
    mat4 bernsteinBasis = mat4(0.0f);
    bernsteinBasis[0][0] = 1.0f;

    float u = 1.0 - t;

    for (int j = 1; j <= n; j++)
    {
        bernsteinBasis[j][0] = bernsteinBasis[j - 1][0] * u;

        for (int i = 1; i <= j; i++)
        {
            bernsteinBasis[j][i] = bernsteinBasis[j - 1][i] * u + bernsteinBasis[j - 1][i - 1] * t;
        }
    }

    return vec4(bernsteinBasis[n][0], bernsteinBasis[n][1], bernsteinBasis[n][2], bernsteinBasis[n][3]);
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
    int uPatch = gl_PrimitiveID / patchCountWidth;
    int vPatch = gl_PrimitiveID % patchCountWidth;
    outU = (u + uPatch) / patchCountLength;
    outV = (v + vPatch) / patchCountWidth;
    //    outU = float(gl_PrimitiveID) / (gridCountLength * gridCountWidth);
    //    outV = 0;
}