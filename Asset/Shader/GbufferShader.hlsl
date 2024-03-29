
#include"Common.hlsl"
#include"Light.hlsl"


Texture2D MainTexture : register(t0);
SamplerState MainTexture_Sampler : register(s0);

struct  VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv:TEXCOORD;
};


struct PSInput
{
    float4 position : SV_POSITION;
    float4 positionW : POSITION;
    float4 normalW : NORMAL;
    float2 uv   :TEXCOORD0;
};

struct Gbuffer
{
    float4 positionW :  SV_TARGET0;
    float4 normalW : SV_TARGET1;
    float4 albedo : SV_TARGET2;

};


//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;


    result.position = ObjectToClipProjection(float4(input.position, 1.0f));
    result.normalW = ObjectToWorldNormal(input.normal);
    //result.position = float4(0, 0, 0, 0);
    result.uv = input.uv;
    result.positionW = ObjectToWorldPoint(float4(input.position, 1.0f));
    return result;
}

Gbuffer PsMain(PSInput input) : SV_TARGET
{

    float4 albedo = MainTexture.Sample(MainTexture_Sampler, input.uv);
    
    Gbuffer gbuffer;
    gbuffer.positionW = input.positionW;
    gbuffer.normalW = input.normalW  ;
    gbuffer.albedo = albedo;
    

    return gbuffer;
    //return color;
//    return float4(1, 1, 1, 1);
}