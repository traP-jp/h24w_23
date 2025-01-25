struct PS_Input
{
    float4 pos : SV_POSITION;
};

PS_Input vs(float4 pos : POSITION)
{
    PS_Input vsOut;
    vsOut.pos = pos;
    return vsOut;
}

float4 ps(PS_Input psIn) : SV_TARGET
{
    return float4(0.7f, 0.7f, 0.7f, 0.7f);
}