#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cstdlib>
#include <float.h>
#include <exception>


Texture::Texture(const char* filename)
{
	_filename = filename;
}


Texture::~Texture()
{
	if (_texture != nullptr) {
		_texture->Release();
		_texture = nullptr;
	}

	if (_sampler != nullptr) {
		_sampler->Release();
		_sampler = nullptr;
	}
}

void Texture::init(ID3D11Device* device)
{
	createTexture(device);
	createSampler(device);
}

void Texture::createTexture(ID3D11Device* device)
{
	int width = 0;
	int height = 0;
	int channels = 0;

	byte* pixels = stbi_load(_filename, &width, &height, &channels, 4);

	if (pixels == nullptr) {
		throw new std::exception("Failed to load texture. Texture.cpp:createTexture()");
	}

	ID3D11Texture2D* texture = nullptr;
	D3D11_TEXTURE2D_DESC desc {};
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Width = width;
	desc.Height = height;
	desc.SampleDesc.Count = 1;
	desc.MipLevels = 1;

	D3D11_SUBRESOURCE_DATA data {};
	data.pSysMem = pixels;
	data.SysMemPitch = width * 4;

	HRESULT hr = device->CreateTexture2D(&desc, &data, &texture);

	if (FAILED(hr)) {
		delete[] pixels;
		pixels = nullptr;

		throw new std::exception("Failed to create 2d texture. Texture.cpp:createTexture()");
	}

	hr = device->CreateShaderResourceView(texture, nullptr, &_texture);

	texture->Release();
	texture = nullptr;
	delete[] pixels;
	pixels = nullptr;

	if (FAILED(hr)) {
		throw new std::exception("Failed to create shader resource view. Texture.cpp:createTexture()");
	}
}

void Texture::createSampler(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC desc {};
	desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MinLOD = -FLT_MIN;
	desc.MaxLOD = FLT_MAX;

	if (FAILED(device->CreateSamplerState(&desc, &_sampler))) {
		throw new std::exception("Failed to create sampler state. Texture.cpp:createSampler()");
	}
}
