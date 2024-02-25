#version 460 core

in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragView;
in mat4 fragProj;
in vec2 texCoord;
out vec4 outColor;

uniform float near;
uniform float far;

struct PointLight {
    vec3 position;
    vec3 color;
    float strength;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};
uniform PointLight pointLight;

uniform float specularFactor;

uniform vec3 viewPos;
uniform sampler2D coordinatesTexture;
uniform int pixelSize;
uniform int viewportOffsetX;
uniform int viewportOffsetY;

uniform mat4 D;


vec3 calculateLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = max(normalize(lightDir + viewDir), 0);
    float spec = specularFactor != 0 ? pow(max(dot(normal, halfwayDir), 0.0), specularFactor) : 0;
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / ((distance * distance)* light.quadraticAttenuation + distance*light.linearAttenuation + light.constantAttenuation);
    // combine results
    vec3 ambient  = light.color * vec3(0.2f);
    vec3 diffuse  = light.color * diff;
    vec3 specular = light.color * spec;
    return ambient + (diffuse + specular) * light.strength * attenuation;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

void main() {
    vec3 fragPos3D = texelFetch(coordinatesTexture, ivec2((gl_FragCoord.xy - vec2(viewportOffsetX, viewportOffsetY)) / pixelSize)*pixelSize, 0).xyz;
    if(fragPos3D.x == 0 && fragPos3D.y == 0 && fragPos3D.z == 0) discard;

    gl_FragDepth = ((gl_DepthRange.diff * computeDepth(fragPos3D)) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

    vec4 v = vec4(fragPos3D, 1);
    vec3 normal = vec3(0);

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++)
            normal[i] += (D[i][j] + D[j][i]) * v[j];
    }

    vec3 viewDir = normalize(viewPos - fragPos3D);
    vec3 result = vec3(1,1,0) * calculateLight(pointLight, normalize(normal), fragPos3D, viewDir);


    outColor = vec4(result,1);
}
