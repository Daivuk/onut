input float2 inPosition;
input float2 inUV;
input float4 inColor;

output float2 outUV;
output float4 outColor;

extern float anim;

void main()
{
    oPosition = mul(float4(inPosition.x, inPosition.y + sin(anim) * 100, 0, 1), oViewProjection);
    outUV = inUV;
    outColor = inColor;
}
