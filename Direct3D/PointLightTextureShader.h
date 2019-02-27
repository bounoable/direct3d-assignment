#pragma once
#include "TextureShader.h"

class PointLightTextureShader :
	public TextureShader
{
public:
	PointLightTextureShader(Texture* texture, LPCTSTR vsName = TEXT("PointLightTextureVertexShader.hlsl"), LPCTSTR psName = TEXT("PointLightTexturePixelShader.hlsl"));
	~PointLightTextureShader();

	void init(ID3D11Device* device);

	void setLightProperties(XMFLOAT3 position, FLOAT radius, FLOAT intensity, XMFLOAT4 color)
	{
		_lightBuffer.position = position;
		_lightBuffer.radius = radius;
		_lightBuffer.intensity = intensity;
		_lightBuffer.color = color;
	}

protected:
	void createLightBuffer(ID3D11Device* device);
	void setShaderParameters(ID3D11DeviceContext* deviceContext);

	struct LightBuffer
	{
		XMFLOAT3 position = {};
		FLOAT radius = 0.0f;
		XMFLOAT4 color = {};
		FLOAT intensity = 0.0f;
		XMFLOAT3 padding = {};
	};

	LightBuffer _lightBuffer = {};
	ID3D11Buffer* _d3dLightBuffer = nullptr;
};

