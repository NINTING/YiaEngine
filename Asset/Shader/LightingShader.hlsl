
#include"Common.hlsl"
#include"Light.hlsl"


Texture2D PostionWTexture : register(t0);
Texture2D NormalWTexture : register(t1);
Texture2D UvTexture : register(t2);
Texture2D MainTexture : register(t3);

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

struct Gbuffer
{
    float4 positionW :  SV_TARGET0;
    float4 normalW : SV_TARGET1;
    float4 uv : SV_TARGET2;

};


//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;

    result.position = float4(input.position, 1.0f);
    result.uv = input.uv;

    return result;
}

Gbuffer PsMain(PSInput input) : SV_TARGET
{
     float4 posW = PostionWTexture.Sample(Common_Sampler,input.uv);
     float4 normalW = NormalWTexture.Sample(Common_Sampler, input.uv);
     float4 UV = UvTexture.Sample(Common_Sampler, input.uv);]

     float4 albedo = MainTexture.Sample(Common_Sampler,UV);
     float3 diffuse = ComputeDirectionLight(Lights, surface, posW.xyz, normalW.xyz);

    return gbuffer;
    //return color;
//    return float4(1, 1, 1, 1);
}