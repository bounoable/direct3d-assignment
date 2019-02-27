cbuffer MatrixBuffer
{
    matrix worldViewProjectionMatrix;
    matrix worldMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 view : TEXCOORD1;
};

VertexOutput main(VertexInput IN)
{
    VertexOutput OUT = (VertexOutput) 0;

    IN.position.w = 1.0f;
    IN.normal.w = 0.0f;

    OUT.uv = IN.uv;
    OUT.position = mul(IN.position, worldViewProjectionMatrix);
    OUT.normal = mul(IN.normal, worldMatrix);
    OUT.view = float3(0.0f, 0.0f, -2.0f) - mul(IN.position, worldMatrix).xyz;

    return OUT;
}
