Texture2D texDiffuse : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer Cartoon : register(b0)
{
    float3 tone;
    float padding;
}

float4 main(PSInput input) : SV_TARGET
{
    float4 diffuse = texDiffuse.Sample(samplerState, input.texCoord);

    diffuse.rgb = round(diffuse.rgb * tone) / tone;

    return diffuse;
}
