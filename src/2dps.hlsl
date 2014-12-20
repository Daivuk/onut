Texture2D texDiffuse : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 diffuse = texDiffuse.Sample(samplerState, input.texCoord);

    return diffuse * input.color;
}
