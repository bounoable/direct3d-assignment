#pragma once
#include "AShader.h"
#include "Texture.h"

class TextureShader: public AShader
{
public:
	TextureShader(Texture* texture, LPCTSTR vsName = TEXT("TextureVertexShader.hlsl"), LPCTSTR psName = TEXT("TexturePixelShader.hlsl"));
	~TextureShader();

	Texture* getTexture()
	{
		return _texture;
	}

	void setTexture(Texture* texture)
	{
		if (_texture != nullptr) {
			delete _texture;
		}

		_texture = texture;
	}

protected:
	Texture* _texture = nullptr;

	void createInputLayout(ID3D11Device* device, ID3DBlob* blob);
	void setShaderParameters(ID3D11DeviceContext* deviceContext);
};

