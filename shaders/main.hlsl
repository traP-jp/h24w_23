struct Type {
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

Type vs(
    float4 position : POSITION,
    float2 uv : TEXCOORD,
    float4 normal : NORMAL
)
{
    Type output;
    output.position = mul(projection, mul(view, mul(world, position)));
    output.uv = uv;
    output.normal = mul(world, normal);

    return output;
}

Texture2D tex : register(t0);
SamplerState sam : register(s0);

float4 ps(Type input) : SV_TARGET
{
	float3 light = normalize(float3(0.0, -1.0, -1.0));
    float3 normal = normalize(input.normal.xyz);
	float brightness = dot(normal, light);

    float2 uv = input.uv;
    uv.y = 1.0 - uv.y + 0.001;
    uv.x = uv.x + 0.001;
	return tex.Sample(sam, uv) * brightness;
}
