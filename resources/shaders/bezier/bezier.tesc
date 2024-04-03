#version 460 core

layout (vertices = 4) out;

uniform int adaptationMultiplier;
uniform int windowWidth;
uniform int windowHeight;
uniform mat4 projection;
uniform mat4 view;

patch out float patchColorek;

in int instanceID_vert[32];
patch out int instanceID;

int calculateSegmentCount() {
    float l = 0;
    vec2 p[4];

    for(int i=0; i<4; i++) {
        vec4 pos = (projection * view * vec4(gl_in[i].gl_Position.xyz, 1));
        vec2 scPos = pos.xy / pos.w;
        p[i] = vec2(scPos.x * windowWidth, scPos.y * windowHeight);
    }

    for(int i=0; i<4; i++) {
        int iNext = (i+1)%4;
        l += length(p[iNext] - p[i]);
    }
    return int(l/adaptationMultiplier);
}

void main()
{
    int n = 256;
    int temp = calculateSegmentCount();
    int segments = temp <= n ? temp : n;

    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = segments;

    patchColorek = float(segments) / n;

    instanceID = instanceID_vert[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}