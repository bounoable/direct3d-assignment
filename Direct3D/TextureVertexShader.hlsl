cbuffer MatrixBuffer
{
    matrix worldViewProjectionMatrix;
    matrix worldMatrix;
};

struct VertexInput
{
	
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 binormal : BINORMAL;
    float4 tangent : TANGENT;
    float2 uv: TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput main(VertexInput IN)
{
    VertexOutput OUT = (VertexOutput) 0;

    IN.position.w = 1.0f;
    OUT.uv = IN.uv;
    OUT.position = mul(IN.position, worldViewProjectionMatrix);

    return OUT;
}
