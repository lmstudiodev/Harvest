cbuffer hcbuffer : register(b0)
{
    float4x4 mat;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTextCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_Position;
    float2 outTextCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.outPosition = mul(float4(input.inPos, 1.0f), mat);
    output.outTextCoord = input.inTextCoord;
    
    return output;
}