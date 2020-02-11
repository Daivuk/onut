input float2 inPosition;
input float2 inUV;
input float4 inColor;

output float4 outColor;

Texture0 texture0;

void main()
{
    float2 uv = inUV;
    outColor = texture0(uv);
    float yOffset = outColor.g;
    oPosition = mul(float4(inPosition.x, inPosition.y + yOffset * 64.0, 0, 1), oViewProjection);
}
