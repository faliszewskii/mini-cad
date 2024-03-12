#version 460 core
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 256) out;

uniform int adaptationMultiplier;
uniform int windowWidth;
uniform int windowHeight;
uniform mat4 projection;
uniform mat4 view;

out float colorek; // TODO DEBUG

vec3 deCasteljau(float t, vec3 coefficients[4]) {
    int n = coefficients.length();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < (n - i); j++) {
            coefficients[j] = coefficients[j] * (1 - t) + coefficients[j + 1] * t;
        }
    }
    return coefficients[0];
}

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
    int segmentCount = calculateSegmentCount();
    int count = segmentCount <= 255 ? segmentCount : 255;
    float dt = 1.f / count;
    vec3 coefficients[4] = vec3[](gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);

    for(int i = 0; i < count + 1; i++) {
        vec3 point = deCasteljau(i * dt, coefficients);
        gl_Position = projection * view * vec4(point, 1);
        colorek = (segmentCount <= 255 ? segmentCount : 255)/255.f; // TODO DEBUG
        EmitVertex();
    }
    EndPrimitive();
}