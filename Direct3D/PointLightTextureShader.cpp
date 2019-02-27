#include "PointLightTextureShader.h"
#include <exception>


PointLightTextureShader::PointLightTextureShader(Texture* texture, LPCTSTR vsName, LPCTSTR psName): TextureShader(texture, vsName, psName)
{
}

PointLightTextureShader::~PointLightTextureShader()
{
	if (_d3dLightBuffer != nullptr) {
		_d3dLightBuffer->Release();
		_d3dLightBuffer = nullptr;
	}

	TextureShader::~TextureShader();
}

void PointLightTextureShader::init(ID3D11Device* device)
{
	TextureShader::init(device);

	createLightBuffer(device);
}

void PointLightTextureShader::createLightBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC desc {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(LightBuffer);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&desc, nullptr, &_d3dLightBuffer))) {
		throw new std::exception("Failed to create light buffer. PointLightTextureShader.cpp:createLightBuffer()");
	}
}

void PointLightTextureShader::setShaderParameters(ID3D11DeviceContext* deviceContext)
{
	TextureShader::setShaderParameters(deviceContext);

	D3D11_MAPPED_SUBRESOURCE buffer = {};

	if (FAILED(deviceContext->Map(_d3dLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer))) return;

	*reinterpret_cast<LightBuffer*>(buffer.pData) = _lightBuffer;

	deviceContext->Unmap(_d3dLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &_d3dLightBuffer);
}
