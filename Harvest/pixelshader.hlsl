struct PS_INPUT
{
    float4 inPosition : SV_Position;
    float2 inTextCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 pixelColor = objTexture.Sample(objSamplerState, input.inTextCoord);
        
    return float4(pixelColor, 1.0f);
}