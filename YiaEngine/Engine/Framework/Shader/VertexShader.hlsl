
struct  VSInput
{
    float3 position : POSITION;
   
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
   
    float2 uv   :TEXCOORD;
};


PSInput VSMain(VSInput input)
{
    PSInput result;

    result.position =float4(input.position,0);
  
    result.uv = input.uv;
    return result;
}

