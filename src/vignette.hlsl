Texture2D texDiffuse : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer vignette : register(b0)
{
    float2 kernelSize;
    float amount;
    float padding;
}

float4 VignettePass(float4 colorInput, float2 tex)
{
    tex = tex * 2 - 1;
    float dis = length(tex);
    dis = dis / 1.4142135623730950488016887242097;
    dis = pow(dis, 3);
    colorInput.rgb *= pow(1 - dis * amount, 2);
    return colorInput;

    return colorInput;
}

/* --- Main --- */

float4 main(PSInput input) : SV_TARGET
{
    float4 c0 = texDiffuse.Sample(samplerState, input.texCoord);

    c0 = VignettePass(c0, input.texCoord);
    return c0;
}
