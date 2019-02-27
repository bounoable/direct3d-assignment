cbuffer MatrixBuffer
{
	matrix worldViewProjectionMatrix;
};

struct VertexInput
{
	
	float4 position: POSITION;
	float4 color: COLOR;
};

struct VertexOutput
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;

	OUT.color = IN.color;
	OUT.position = mul(IN.position, worldViewProjectionMatrix);

	return OUT;
}
