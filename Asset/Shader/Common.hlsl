#define UPDATE_FREQ_NONE        space0
#define UPDATE_FREQ_PER_FRAME   space1
#define UPDATE_FREQ_PER_BATCH   space2
#define UPDATE_FREQ_PER_DRAW    space3

struct Surface
{
    float4 Albedo;
   
};


cbuffer cbvPerFrame : register(b0, UPDATE_FREQ_PER_FRAME)
{
    float4x4 ObjectMat;
    float4x4 ViewMat;
    float4x4 ProjMat;
}

cbuffer cbvPerDraw : register(b1, UPDATE_FREQ_PER_BATCH)
{
    Surface surface;
};

cbuffer cbvPerDraw : register(b1, UPDATE_FREQ_PER_DRAW)
{
    float4x4 WorldMat;
    float4x4 WorldInverMat;
};


float4 ObjectToClipProjection(float4 p)
{
    //float4x4 wvp =  mul(mul(mul(ObjectMat,WorldMat),ViewMat),ProjMat);
    //float4x4 wvp =mul(mul(mul(ProjMat,ViewMat), WorldMat), ObjectMat);

    float4x4 wo = mul(WorldMat, ObjectMat);
    float4x4 vwo = mul(ViewMat, wo);
    float4x4 pvwo = mul(ProjMat, vwo);
    return mul(pvwo, p);
}
float4 ObjectToWorldPoint(float4 p)
{  
    return mul(WorldMat, p);
}
float4 WorldToClip(float4 p)
{
    float4x4 vwo = mul(ViewMat, p);
    float4x4 pvwo = mul(ProjMat, vwo);
    return mul(pvwo, p);
}
float4 ObjectToWorldNormal(float3 normal)
{
    return mul(float(normal, 0.f), WorldInverMat);
}
