#pragma once
#include "TextureShader.h"

class LightTextureShader :
	public TextureShader
{
public:
	LightTextureShader(Texture* texture, LPCTSTR vsName = TEXT("LightTextureVertexShader.hlsl"), LPCTSTR psName = TEXT("LightTexturePixelShader.hlsl"));
	~LightTextureShader();

	void init(ID3D11Device* device);

	void setLightProperties(XMFLOAT3 direction, FLOAT intensity, XMFLOAT4 color)
	{
		_lightBuffer.direction = direction;
		_lightBuffer.intensity = intensity;
		_lightBuffer.color = color;
	}

protected:
	void createLightBuffer(ID3D11Device* device);
	void setShaderParameters(ID3D11DeviceContext* deviceContext);

	struct LightBuffer
	{
		XMFLOAT3 direction = {};
		FLOAT intensity = 0.0f;
		XMFLOAT4 color = {};
	};

	LightBuffer _lightBuffer = {};
	ID3D11Buffer* _d3dLightBuffer = nullptr;
};

