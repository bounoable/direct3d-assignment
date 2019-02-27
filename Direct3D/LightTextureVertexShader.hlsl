cbuffer MatrixBuffer
{
    matrix worldViewProjectionMatrix;
    matrix worldMatrix;
    float4 cameraPosition;
};

struct VertexInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 binormal : BINORMAL;
    float4 tangent : TANGENT;
    float3 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL;
    float3 uv : TEXCOORD;
    float3 view : TEXCOORD1;
};

VertexOutput main(VertexInput IN)
{
    VertexOutput OUT = (VertexOutput) 0;

    IN.position.w = 1.0f;
    IN.normal.w = 0.0f;

    OUT.uv = IN.uv;
    OUT.positionW = mul(IN.position, worldMatrix).xyz;
    OUT.position = mul(IN.position, worldViewProjectionMatrix);
    OUT.normal = mul(IN.normal, worldMatrix);
    OUT.view = cameraPosition.xyz - mul(IN.position, worldMatrix).xyz;

    return OUT;
}
