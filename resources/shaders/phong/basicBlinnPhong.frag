#version 460 core
out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform bool useTexture;
uniform sampler2D texture_diffuse;
uniform vec3 viewPos;
uniform vec3 backgroundColor;

void main()
{
    vec4 objectColor = useTexture? texture(texture_diffuse, texCoord): vec4(1);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 normNormal = normalize(normal);
    vec3 result = backgroundColor;

    // Directional Light
//    result += (useDirectionalLight == true)? calcDirLight(dirLight, normNormal, viewDir): vec3(0.f);
    // Point Lights
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
//    result += calcPointLight(pointLights[i], normNormal, fragPos, viewDir);
    // Spot Lights
//    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
//    result += calcSpotLight(spotLights[i], normNormal, fragPos, viewDir);

    result *= objectColor.rgb;
//    if(fog.useFog == true)
//    result = mix(result, fog.color, getFogFactor(fog, length(viewPos - fragPos)));
    fragColor = vec4(result, objectColor.a);
}