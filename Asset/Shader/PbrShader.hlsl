
#include"Common.hlsl"
#include"Light.hlsl"

#define Renderer_RootSig \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX) " \

Texture2D MainTexture : register(t0);
SamplerState MainTexture_Sampler : register(s0);






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
    float2 uv       :TEXCOORD;
    float3 normal   :NORMAL;
};
//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;


    result.position = ObjectToClipProjection(float4(input.position, 1.0f));
    result.normalW = ObjectToWorldNormal(float4(input.normal, 1.0f)).xyz;
    //result.position = float4(0, 0, 0, 0);
    result.uv = input.uv;
    result.posW = ObjectToWorldPoint(float4(input.position, 1.0f));
    return result;
}

float4 PsMain(PSInput input) : SV_TARGET
{
    float4 albedo = MainTexture.Sample(MainTexture_Sampler,input.uv);
    float3 diffuse = ComputeDirectionLight(Lights, surface, input.posW.xyz, input.normalW);
    return float4(diffuse*albedo.xyz,1.f);
    //return albedo;
  //return MainTexture.Sample(MainTexture_Sampler,float2(input.uv.x,input.uv.y));
}