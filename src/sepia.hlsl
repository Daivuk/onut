Texture2D texDiffuse : register(t0);
SamplerState samplerState : register(s0);

cbuffer Sepia : register(b0)
{
    float3 tone;
    float desaturate;
    float sepiaAmount;
    float3 padding;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 colorInput = texDiffuse.Sample(samplerState, input.texCoord);
    float3 sepia = colorInput.rgb;

    // calculating amounts of input, grey and sepia colors to blend and combine
    float grey = dot(sepia, float3(0.2126, 0.7152, 0.0722));

    float3 grayscale = (grey * desaturate) + (colorInput.rgb * (1 - desaturate));

    sepia = lerp(sepia, grayscale, sepiaAmount);
    sepia *= tone;

    colorInput.rgb = lerp(grayscale, sepia, sepiaAmount);

    // returning the final color
    return colorInput;
}
