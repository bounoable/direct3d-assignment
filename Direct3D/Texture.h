#pragma once
#include <d3d11.h>
#include <Windows.h>

class Texture
{
public:
	Texture(const char* filename);
	~Texture();


	void init(ID3D11Device* device);

	const char* getFilename()
	{
		return _filename;
	}

	ID3D11ShaderResourceView* getTexture()
	{
		return _texture;
	}

	ID3D11SamplerState* getSampler()
	{
		return _sampler;
	}

private:
	void createTexture(ID3D11Device* device);
	void createSampler(ID3D11Device* device);

	const char* _filename = nullptr;
	ID3D11ShaderResourceView* _texture = nullptr;
	ID3D11SamplerState* _sampler = nullptr;
};

