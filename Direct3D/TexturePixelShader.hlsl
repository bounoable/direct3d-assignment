Texture2D mainTexture;
sampler mainSampler;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput IN) : SV_TARGET
{
    float4 color = mainTexture.Sample(mainSampler, IN.uv);

    return color;
}
