#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class AShader
{
public:
	AShader(LPCTSTR vsName, LPCTSTR psName);
	virtual ~AShader();

	virtual void init(ID3D11Device* device);
	void render(ID3D11DeviceContext* deviceContext, USHORT indexCount);

	void setWorldViewProjectionMatrix(XMFLOAT4X4 world, XMFLOAT4X4 wvp, XMFLOAT3 cameraPosition)
	{
		_matrixBuffer.worldMatrix = world;
		_matrixBuffer.worldViewProjectionMatrix = wvp;
		_matrixBuffer.cameraPosition = XMFLOAT4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
	}

protected:
	void createVertexShader(ID3D11Device* device);
	void createPixelShader(ID3D11Device* device);
	virtual void createInputLayout(ID3D11Device* device, ID3DBlob* blob) = 0;

	void createMatrixBuffer(ID3D11Device* device);
	void setMatrixBuffer(ID3D11DeviceContext* deviceContext);
	virtual void setShaderParameters(ID3D11DeviceContext* deviceContext) {};

	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3D11InputLayout* _inputLayout = nullptr;
	ID3D11Buffer* _d3dMatrixBuffer = nullptr;

	LPCTSTR _vertexShaderName = TEXT("");
	LPCTSTR _pixelShaderName = TEXT("");

	XMFLOAT4X4 _worldViewProjectionMatrix;

	struct MatrixBuffer
	{
		XMFLOAT4X4 worldViewProjectionMatrix;
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4 cameraPosition; // put here for convenience
	};

	MatrixBuffer _matrixBuffer = {};
};
