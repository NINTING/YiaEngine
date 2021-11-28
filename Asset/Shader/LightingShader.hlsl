
#include"Common.hlsl"
#include"Light.hlsl"


Texture2D PostionWTexture : register(t0);
Texture2D NormalWTexture : register(t1);
Texture2D AlbedoTexture : register(t2);

SamplerState Common_Sampler : register(s0);

struct  VSInput
{
    float3 position : POSITION;
    float2 uv:TEXCOORD;
};


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv   :TEXCOORD0;
};



//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;

    result.position = float4(input.position, 1.0f);
    result.uv = input.uv;

    return result;
}

float4 PsMain(PSInput input) : SV_TARGET
{
     float4 posW = PostionWTexture.Sample(Common_Sampler,input.uv);
     float4 normalW = NormalWTexture.Sample(Common_Sampler, input.uv);
     float4 albedo = AlbedoTexture.Sample(Common_Sampler, input.uv);
     Surface finalSurface = surface;
     finalSurface.Albedo *= albedo;
     float3 diffuse = ComputeDirectionLight(Lights, finalSurface, posW.xyz, normalW.xyz);

    return float4( diffuse + finalSurface.Albedo * 0.1,1.f);
    //return color;
//    return float4(1, 1, 1, 1);
}