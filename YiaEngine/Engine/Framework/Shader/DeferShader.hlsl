
Texture2D t1 : register(t0);
SamplerState s1 : register(s0);


cbuffer ConstBufferPerFrame : register(b0)
{
    float4x4 vpMat;
}

struct PSInput
{
    float4 position : SV_POSITION;
 
    float3 uv   :TEXCOORD;
};


struct  VSInput
{
    float3 position : POSITION;
    float3 uv:TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput result;

    //result.position = float4(input.position,1.0f);
    result.position = mul(vpMat,float4(input.position, 1.0f));
    result.uv = input.uv;
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    //  float4 color = float4(1,1,1,1);
    return t1.Sample(s1,float2( input.uv.x,input.uv.y));
    return float4(input.uv.xy, 0.f, 0.f);
    //return color;
}