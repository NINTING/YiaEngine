
#include"Common.hlsl"

Texture2D MainTexture : register(t0);
SamplerState MainTexture_Sampler : register(s0);


//cbuffer ConstBufferPerFrame : register(b0)
//{
//    float4x4 vpMat;
//}

struct PSInput
{
    float4 position : SV_POSITION;

    float2 uv   :TEXCOORD0;

};


struct  VSInput
{
    float3 position : POSITION;

    float2 uv:TEXCOORD;
};

PSInput VsMain(VSInput input)
{
    PSInput result;

    result.position = float4(input.position,1.0f);
 //   result.position = mul(vpMat, float4(input.position, 1.0f));
    result.uv = input.uv;
    return result;
}

float4 PsMain(PSInput input) : SV_TARGET
{
    float4 color = MainTexture.Sample(MainTexture_Sampler,float2(input.uv.x,input.uv.y));
    return color;
    
}