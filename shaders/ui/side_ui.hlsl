cbuffer WorldMatrix : register(b0)
{
    matrix world;
};

cbuffer Material : register(b1)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

cbuffer CameraMatrix : register(b2)
{
    matrix view;
    matrix projection;
};

cbuffer Scroll : register(b3)
{
    float scroll;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

PS_Input vs(
    float4 pos : POSITION, 
    float2 uv : TEXCOORD, 
    float4 normal : NORMAL
)
{
    PS_Input vsOut;
    vsOut.pos = mul(projection, mul(view, mul(world, pos)));
    vsOut.uv = uv;
    vsOut.normal = mul(world, normal);
    return vsOut;
}

Texture2D alpha1_tex : register(t0);
Texture2D alpha2_tex : register(t1);
SamplerState sam : register(s0);

float4 ps(PS_Input psIn) : SV_TARGET
{
    float3 light = normalize(float3(0.0, -1.0, -1.0));
    float brightness = dot(psIn.normal, light);

    float4 alpha1 = alpha1_tex.Sample(sam, psIn.uv + float2(0.0, scroll));
    float4 alpha2 = alpha2_tex.Sample(sam, psIn.uv);

    float4 color = alpha1 * alpha2;
    color.a = color.r;
    return color;
}