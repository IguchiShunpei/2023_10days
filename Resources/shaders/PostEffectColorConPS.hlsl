#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
  
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv) * color;
    float3 c;

    if(isPostE == true){
        c = 1 - col.rgb;
    }

    return float4(c, 1);
}