#ifndef YIA_HLSL_LIGHT
#define YIA_HLSL_LIGHT
#include"Common.hlsl"



#define MAX_LIGHT_COUNT 16

struct Light
{
    float3 LightDir;
    float Intensity;
    float4 LightColor;
};

#define MAX_LIGHT_COUNT 16

cbuffer cbvLight : register(b1, UPDATE_FREQ_PER_FRAME)
{
    Light Lights;

}


float3 ComputeDirectionLight(Light light,Surface surface,
                    float3 posW,float3 normal)
{
    float lambert = max(0, dot(-light.LightDir, normal));

    float3 diffuse = lambert * light.LightColor.xyz * surface.Albedo.xyz * Lights.Intensity;
    return diffuse;
}



#endif //YIA_HLSL_LIGHT