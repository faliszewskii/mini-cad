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
    int n = 4;

    for(int i=0; i<n; i++) {
        vec4 pos = (projection * view * vec4(gl_in[i].gl_Position.xyz, 1));
        vec2 scPos = pos.xy / pos.w;
        p[i] = vec2(scPos.x * windowWidth, scPos.y * windowHeight);
    }

    for(int i=0; i<n; i++) {
        int iNext = (i+1)%n;
        l += length(p[iNext] - p[i]);
    }
    return int(l/adaptationMultiplier);
}

void main()
{
    int temp = calculateSegmentCount();
    int segments = temp <= 255 ? temp : 255;

    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = segments;

    patchColorek = segments / 255.f;

    instanceID = instanceID_vert[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}