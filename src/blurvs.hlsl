struct VSInput
{
    float2 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = float4(input.position.xy, 0, 1);
    output.texCoord.x = (input.position.x + 1) / 2;
    output.texCoord.y = 1 - (input.position.y + 1) / 2;

    return output;
}
