#include "AShader.h"
#include <d3dcompiler.h>
#include <exception>


AShader::AShader(LPCTSTR vsName, LPCTSTR psName): _vertexShaderName(vsName), _pixelShaderName(psName)
{
}


AShader::~AShader()
{
	if (_vertexShader != nullptr) {
		_vertexShader->Release();
		_vertexShader = nullptr;
	}

	if (_pixelShader != nullptr) {
		_pixelShader->Release();
		_pixelShader = nullptr;
	}

	if (_inputLayout != nullptr) {
		_inputLayout->Release();
		_inputLayout = nullptr;
	}

	if (_d3dMatrixBuffer != nullptr) {
		_d3dMatrixBuffer->Release();
		_d3dMatrixBuffer = nullptr;
	}
}

void AShader::init(ID3D11Device* device)
{
	createVertexShader(device);
	createPixelShader(device);
	createMatrixBuffer(device);
}

void AShader::render(ID3D11DeviceContext* deviceContext, USHORT indexCount)
{
	setMatrixBuffer(deviceContext);
	setShaderParameters(deviceContext);

	deviceContext->IASetInputLayout(_inputLayout);
	deviceContext->VSSetShader(_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(_pixelShader, nullptr, 0);
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void AShader::createVertexShader(ID3D11Device* device)
{
	ID3DBlob* blob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		_vertexShaderName,
		nullptr,
		nullptr,
		"main",
		"vs_4_0",
		0, 0,
		&blob,
		nullptr
	);

	if (FAILED(hr)) {
		std::exception("Failed to compile vertex shader. AShader.cpp:createVertexShader()");
	}

	hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &_vertexShader);

	if (FAILED(hr)) {
		std::exception("Failed to create vertex shader. AShader.cpp:createVertexShader()");
	}

	createInputLayout(device, blob);

	blob->Release();
	blob = nullptr;
}

void AShader::createPixelShader(ID3D11Device* device)
{
	ID3DBlob* blob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		_pixelShaderName,
		nullptr,
		nullptr,
		"main",
		"ps_4_0",
		0, 0,
		&blob,
		nullptr
	);

	if (FAILED(hr)) {
		std::exception("Failed to compile pixel shader. AShader.cpp:createPixelShader()");
	}

	hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &_pixelShader);

	if (FAILED(hr)) {
		std::exception("Failed to create pixel shader. AShader.cpp:createPixelShader()");
	}
}

void AShader::createMatrixBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC desc {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(MatrixBuffer);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&desc, nullptr, &_d3dMatrixBuffer))) {
		std::exception("Failed to create matrix buffer. AShader.cpp:createMatrixBuffer()");
	}
}

void AShader::setMatrixBuffer(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE buffer {};

	if (FAILED(deviceContext->Map(_d3dMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer))) return;

	*reinterpret_cast<MatrixBuffer*>(buffer.pData) = _matrixBuffer;

	deviceContext->Unmap(_d3dMatrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &_d3dMatrixBuffer);
}

