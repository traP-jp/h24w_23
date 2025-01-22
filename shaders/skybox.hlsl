struct VS_INPUT 
{
    float3 position : POSITION;
};

struct VS_OUTPUT 
{
    float4 position : SV_POSITION;
    float3 direction : TEXCOORD;
};

cbuffer CameraMat : register(b0)
{
    matrix view;
    matrix projection;
    float3 eye;
};

cbuffer ObjectMat : register (b1)
{
    matrix world;
};

VS_OUTPUT vs(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), world);
    output.position = mul(mul(projection, view), output.position).xyww;
    output.position.z = 1.0f;
    output.direction = input.position;
    return output;
}

TextureCube cubeTexture : register(t0);
SamplerState cubeSampler : register(s0);

static float exposure = 1.0f;

float ReinhardToneMapping(float l)
{
    return (l * exposure) / (1.0f + l * exposure);
}

static float acesa = 2.51f;
static float acesb = 0.03f;
static float acesc = 2.43f;
static float acesd = 0.59f;
static float acese = 0.14f;

float ACESToneMapping(float l)
{
   return ((l * (acesa * l + acesb)) / (l * (acesc * l + acesd) + acese));
}

static float gamma = 1.5f;

float GammaToneMapping(float l)
{
    float mapped = pow(l, 1.0f / gamma);
    return ReinhardToneMapping(mapped);
}

float4 ps(VS_OUTPUT input) : SV_TARGET
{
    float3 color = cubeTexture.Sample(cubeSampler, normalize(input.direction)).rgb;
    float luminance = dot(color, float3(0.299f, 0.587f, 0.114f));

    float toneMappedluminance = ACESToneMapping(luminance);

    float4 finalColor = float4(color * (toneMappedluminance / luminance), 1.0f);

    return finalColor * 12.0f;
    // float4 tex = cubeTexture.Sample(cubeSampler, normalize(input.direction));
    // return float4(tex.xyz, 1.0f);
}