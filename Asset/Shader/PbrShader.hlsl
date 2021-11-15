
#include"Common.h"
#include"Light.h"
#define Renderer_RootSig \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX) " \

Texture2D MainTexture : register(t0);
SamplerState MainTexture_Sampler : register(s0);

cbuffer cbvPerFrame : register(b0,)
{
    float4x4 ObjectMat;
    float4x4 WorldMat;
    float4x4 ViewMat;
    float4x4 ProjMat;

    float4x4 WorldITMat;
}


struct Light
{
    float3 LightDir;
    float Intensity;
    float4 LightColor;
};
#define MAX_LIGHT_COUNT 16
cbuffer cbvLight: register(b1)
{
    Light Lights;

}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 posW : POSITION;
    float3 normalW : NORMAL;
    float2 uv   :TEXCOORD0;
};


struct  VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv:TEXCOORD;
};





//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;
    
    
    result.position =ObjectToClipProjection(float4(input.position,1.0f));
    result.normalW = ObjectToWorldNormal(input.normal);
    //result.position = float4(0, 0, 0, 0);
    result.uv = input.uv;
    result.posW = ObjectToWorldPoint(float4(input.position, 1.0f))
    return result;
}

float4 PsMain(PSInput input) : SV_TARGET
{
    float4 albedo = MainTexture.Sample(MainTexture_Sampler,float2(input.uv.x,input.uv.y));
    ComputeDirectionLight(light, surface, input.posW, input.normalW);
    return diffuse
    //return color;
}