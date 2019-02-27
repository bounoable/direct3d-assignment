#pragma once
#include "TextureShader.h"

class SpotLightTextureShader :
	public TextureShader
{
public:
	SpotLightTextureShader(Texture* texture, LPCTSTR vsName = TEXT("SpotLightTextureVertexShader.hlsl"), LPCTSTR psName = TEXT("SpotLightTexturePixelShader.hlsl"));
	~SpotLightTextureShader();

	void init(ID3D11Device* device);

	void setLightProperties(XMFLOAT3 position, FLOAT radius, FLOAT intensity, XMFLOAT4 color, XMFLOAT3 direction, FLOAT angle)
	{
		_lightBuffer.position = position;
		_lightBuffer.radius = radius;
		_lightBuffer.intensity = intensity;
		_lightBuffer.color = color;
		_lightBuffer.direction = direction;
		_lightBuffer.angle = angle;
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
		XMFLOAT3 direction = {};
		FLOAT angle = 0.0f;
		XMFLOAT3 padding = {};
	};

	LightBuffer _lightBuffer = {};
	ID3D11Buffer* _d3dLightBuffer = nullptr;
};

