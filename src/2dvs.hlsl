cbuffer MatrixBuffer : register(b0)
{
    matrix viewProj2D;
}

struct VSInput
{
    float2 position : POSITION;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = mul(float4(input.position.xy, 0, 1), viewProj2D);
    output.texCoord = input.texCoord;
    output.color = input.color;
   
    return output;
}
