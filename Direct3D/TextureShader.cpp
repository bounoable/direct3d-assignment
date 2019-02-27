#include "TextureShader.h"
#include <exception>



TextureShader::TextureShader(Texture* texture, LPCTSTR vsName, LPCTSTR psName): AShader(vsName, psName)
{
	_texture = texture;
}


TextureShader::~TextureShader()
{
	if (_texture != nullptr) {
		delete _texture;
		_texture = nullptr;
	}

	AShader::~AShader();
}

void TextureShader::createInputLayout(ID3D11Device* device, ID3DBlob* blob)
{
	D3D11_INPUT_ELEMENT_DESC desc[5] {};

	// Position
	desc[0].SemanticName = "POSITION";
	desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	// Normal
	desc[1].SemanticName = "NORMAL";
	desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// Binormal
	desc[2].SemanticName = "BINORMAL";
	desc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// Tangent
	desc[3].SemanticName = "TANGENT";
	desc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// UV
	desc[4].SemanticName = "TEXCOORD";
	desc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	if (FAILED(device->CreateInputLayout(desc, 5, blob->GetBufferPointer(), blob->GetBufferSize(), &_inputLayout))) {
		throw new std::exception("Failed to create input layout. TextureShader.cpp:createInputLayout()");
	}
}

void TextureShader::setShaderParameters(ID3D11DeviceContext* deviceContext)
{
	ID3D11ShaderResourceView* resourceView = _texture->getTexture();
	ID3D11SamplerState* samplers[] { _texture->getSampler() };

	deviceContext->PSSetShaderResources(0, 1, &resourceView);
	deviceContext->PSSetSamplers(0, 1, samplers);
}
