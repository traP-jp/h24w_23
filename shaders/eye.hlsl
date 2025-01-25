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

float4 ps(Type input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
