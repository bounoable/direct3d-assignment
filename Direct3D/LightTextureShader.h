#pragma once
#include "TextureShader.h"
#include "Light.h"
#include <unordered_set>

class LightTextureShader :
	public TextureShader
{
public:
	LightTextureShader(Texture* texture, LPCTSTR vsName = TEXT("LightTextureVertexShader.hlsl"), LPCTSTR psName = TEXT("LightTexturePixelShader.hlsl"));
	~LightTextureShader();

	void init(ID3D11Device* device);

	void addLight(Light light)
	{
		if (_lightCount >= 12) {
			return;
		}

		_lightBuffer.lights[_lightCount] = light;
		_lightCount++;
	}

protected:
	void createLightBuffer(ID3D11Device* device);
	void setShaderParameters(ID3D11DeviceContext* deviceContext);

	unsigned int _lightCount = 0;

	struct LightBuffer
	{
		Light lights[12];
	};

	LightBuffer _lightBuffer = {};
	ID3D11Buffer* _d3dLightBuffer = nullptr;
};

