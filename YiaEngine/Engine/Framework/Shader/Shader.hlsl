
SamplerState gsamLinearClamp : register(s0);
Texture2D gTex :register(t0);

struct  VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv   :TEXCOORD0;
};


PSInput VSMain(VSInput input)
{
    PSInput result;

    result.position =float4(input.position,0);
    result.color = input.color;
    result.uv = input.uv;
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = gTex.Sample(gsamLinearClamp,input.uv);
    return color;
}