#include "VertexShader.hlsl"

cbuffer LightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float dynamicLightColorStrength;
    float3 dynamicLightPosition;
    
    float dynamicLightAttenuation_a;
    float dynamicLightAttenuation_b;
    float dynamicLightAttenuation_c;
    float padding[2];
};

Texture2D modeltexture : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 PS(VSout input) : SV_TARGET
{
    float3 samplecolor = modeltexture.Sample(samplerState, input.uv);
    //float3 samplernormal = input.normal;
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 appliedLight = ambientLight;
    float3 vectorToLight = normalize(dynamicLightPosition - input.wolrdPos);
    float3 diffusIntensity = max(dot(vectorToLight, input.normal), 0);
    float distanceToLight = distance(dynamicLightPosition, input.wolrdPos);
    float attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));
    diffusIntensity *= attenuationFactor;
    float3 diffusLight = diffusIntensity * dynamicLightColorStrength;
    appliedLight += diffusLight;
    float3 finalcolor = samplecolor * appliedLight;
    return float4(finalcolor, 1.0f);
}