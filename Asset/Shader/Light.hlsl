#pragma once
#include"Common.hlsl"




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


float4 ComputeDirectionLight(Light light,Surface surface,
                    float3 posW,float3 normal)
{
    float3 diffuse = max(0, dot(lights.LightDir, normal)) *
        light.LightColor.xyz * albedo.xyz * Lights.Intensity;
    return diffuse;
}



