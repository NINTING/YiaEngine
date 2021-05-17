
Texture2D t1 : register(t0);
SamplerState s1 : register(s0);


struct PSInput
{
    float4 position : SV_POSITION;
   
    float2 uv   :TEXCOORD;
};



float4 PSMain(PSInput input) : SV_TARGET
{
   // float4 color = gTex.Sample(gsamLinearClamp,input.uv);
    return t1.Sample(s1, input.uv);
}