struct PSInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer SceneMatrix : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer WorldMatrix : register(b1)
{
    matrix world;
};

cbuffer Material : register(b2)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

PSInput vs(
    float4 position : POSITION,
    float2 uv : TEXCOORD,
    float4 normal : NORMAL
)
{
    PSInput output;
    output.position = mul(projection, mul(view, mul(world, position)));
    output.uv = uv;
    output.normal = mul(world, normal);

    return output;
}

Texture2D tex : register(t0);
Texture2D alpha1_tex : register(t1);
SamplerState sam : register(s0);

float4 ps(PSInput input) : SV_TARGET
{
    return tex.Sample(sam, input.uv) * alpha1_tex.Sample(sam, input.uv);
}