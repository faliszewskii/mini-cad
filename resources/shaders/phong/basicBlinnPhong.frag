#version 460 core
out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
//uniform vec3 backgroundColor;

struct Material {
    float shininess;
    vec4 albedo;
    bool useAlbedoTexture;
    sampler2D textureAlbedo;
};
uniform Material material;

struct PointLight {
    vec3 position;
    vec3 color;
    float strength;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};
uniform PointLight pointLight;

vec3 calculateLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = material.shininess != 0 ? pow(max(dot(normal, halfwayDir), 0.0), material.shininess) : 0;
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / ((distance * distance)* light.quadraticAttenuation + distance*light.linearAttenuation + light.constantAttenuation);
    // combine results
    vec3 ambient  = light.color * vec3(0.2f);
    vec3 diffuse  = light.color * diff;
    vec3 specular = light.color * spec;
    return (ambient + diffuse + specular) * light.strength * attenuation;
}

void main()
{
    vec4 objectColor = material.useAlbedoTexture? texture(material.textureAlbedo, texCoord): material.albedo;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 normNormal = normalize(normal);
    vec3 result = vec3(0); // TODO Ignore background?

    result += calculateLight(pointLight, normNormal, fragPos, viewDir);
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