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

cbuffer Light : register(b3)
{
    float4 direction;
    float4 color;
};

Type vs(
    float4 position : POSITION,
    float2 uv : TEXCOORD,
    float4 normal : NORMAL
)
{
    Type output;
    output.position = mul(projection, mul(view, mul(world, position)));
    output.uv.x = uv.x;
    output.uv.y = 1.0 - uv.y;
    //output.normal = mul(world, normal);
    output.normal = normal;

    return output;
}

Texture2D tex : register(t0);
SamplerState sam : register(s0);

float4 ps(Type input) : SV_TARGET
{
	float3 light = normalize(direction.xyz);
    float3 normal = normalize(input.normal.xyz);
	float brightness = -dot(normal, light);
    brightness = abs(brightness);
	return tex.Sample(sam, input.uv) * float4(brightness, brightness, brightness, 1.0f);
}
