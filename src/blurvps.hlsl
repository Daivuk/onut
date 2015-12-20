Texture2D texDiffuse : register(t0);
SamplerState samplerState : register(s0);

cbuffer KernelSize : register(b0)
{
    float2 kernelSize;
    float2 padding;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);

    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y - 6 * kernelSize.y)) * 0.002216f;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y - 5 * kernelSize.y)) * 0.008764;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y - 4 * kernelSize.y)) * 0.026995;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y - 3 * kernelSize.y)) * 0.064759;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y - 2 * kernelSize.y)) * 0.120985;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y - 1 * kernelSize.y)) * 0.176033;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y)) * 0.199471;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y + 6 * kernelSize.y)) * 0.002216f;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y + 5 * kernelSize.y)) * 0.008764;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y + 4 * kernelSize.y)) * 0.026995;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y + 3 * kernelSize.y)) * 0.064759;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y + 2 * kernelSize.y)) * 0.120985;
    color += texDiffuse.Sample(samplerState, float2(input.texCoord.x, input.texCoord.y + 1 * kernelSize.y)) * 0.176033;

    return color;
}
