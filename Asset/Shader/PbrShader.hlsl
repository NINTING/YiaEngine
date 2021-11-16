
#include"Common.hlsl"
//#include"Light.hlsl"

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
    float4 position : POSITION;
    float2 uv       :TEXCOORD;
  //  float4 normal   :TEXCOORD1;
};
//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;


    result.position = ObjectToClipProjection(input.position);
   // result.normalW = ObjectToWorldNormal(input.normal).xyz;
    //result.position = float4(0, 0, 0, 0);
    result.uv = input.uv;
    //result.posW = ObjectToWorldPoint(float4(input.position, 1.0f));
    return result;
}

float4 PsMain(PSInput input) : SV_TARGET
{
    //float4 albedo = MainTexture.Sample(MainTexture_Sampler,float2(input.uv.x,input.uv.y));
    //float3 diffuse = ComputeDirectionLight(Lights, surface, input.posW.xyz, input.normalW);
    //return float4(diffuse,1.f);
    //return color;
  return MainTexture.Sample(MainTexture_Sampler,float2(input.uv.x,input.uv.y));
}