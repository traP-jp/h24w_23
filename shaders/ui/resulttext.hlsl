struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PS_Input vs(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    PS_Input vsOut;
    vsOut.pos = pos;
    vsOut.uv = uv;
    return vsOut;
}

Texture2D ui : register(t0);
SamplerState samp : register(s0);

float4 ps(PS_Input psIn) : SV_TARGET
{
    return ui.Sample(samp, psIn.uv);
}