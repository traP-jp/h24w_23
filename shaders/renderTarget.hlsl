Texture2D gameTexture : register(t0);
SamplerState sam : register(s0);

cbuffer Weight : register(b0)
{
    float gameWeight;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

PSInput vs(float4 position : POSITION, float2 texcoord : TEXCOORD0)
{
    PSInput result;
    result.position = position;
    result.texcoord = texcoord;
    return result;
}

float4 ps(PSInput input) : SV_TARGET
{
    float4 gameColor = gameTexture.Sample(sam, input.texcoord);

    return gameColor * gameWeight;
}