#include "ColorShader.h"
#include <exception>


ColorShader::ColorShader(): AShader(TEXT("ColorVertexShader.hlsl"), TEXT("ColorPixelShader.hlsl"))
{
}


ColorShader::~ColorShader()
{
}

void ColorShader::createInputLayout(ID3D11Device* device, ID3DBlob* blob)
{
	D3D11_INPUT_ELEMENT_DESC desc[2] {};

	// Position
	desc[0].SemanticName = "POSITION";
	desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	// Color
	desc[1].SemanticName = "COLOR";
	desc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	if (FAILED(device->CreateInputLayout(desc, 2, blob->GetBufferPointer(), blob->GetBufferSize(), &_inputLayout))) {
		throw new std::exception("Failed to create input layout. ColorShader.cpp:createInputLayout()");
	}
}
