
Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
 
    float2 uv   :TEXCOORD;
};

struct  VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
    
    float2 uv:TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput result;

    result.position = float4(input.position,1.0f);
 
    result.uv = input.uv;
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    //float4 color = gTex.Sample(gsamLinearClamp,input.uv);
  return t1.Sample(s1, input.uv);
   //return float4(input.uv,0,0);
}