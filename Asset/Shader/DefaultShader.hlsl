
#define Renderer_RootSig \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX) " \

Texture2D MainTexture : register(t0);
SamplerState MainTexture_Sampler : register(s0);

cbuffer ConstBufferPerFrame : register(b0)
{
    float4x4 ObjectMat;
    float4x4 WorldMat;
    float4x4 ViewMat;
    float4x4 ProjMat;
}

cbuffer ConstBufferTest: register(b1)
{
    float4 test;
}

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

float4 ObjectToClipProjection(float4 p)
{
    //float4x4 wvp =  mul(mul(mul(ObjectMat,WorldMat),ViewMat),ProjMat);
    //float4x4 wvp =mul(mul(mul(ProjMat,ViewMat), WorldMat), ObjectMat);
    
    float4x4 wo = mul(WorldMat, ObjectMat);
    float4x4 vwo = mul(ViewMat, wo);
    float4x4 pvwo = mul(ProjMat, vwo);
    return mul(pvwo,p);
}
//[RootSignature(Renderer_RootSig)]
PSInput VsMain(VSInput input)
{
    PSInput result;
    
    
    result.position =ObjectToClipProjection(float4(input.position,1.0f));
    //result.position = float4(0, 0, 0, 0);
    result.uv = input.uv;
    return result;
}

float4 PsMain(PSInput input) : SV_TARGET
{
    
    return MainTexture.Sample(MainTexture_Sampler,float2(input.uv.x,input.uv.y));
    //return color;
}