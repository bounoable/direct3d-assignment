#include "Direct3D.h"
#include <exception>


Direct3D::Direct3D(HWND hWnd, UINT screenWidth, UINT screenHeight)
{
	getOutputFrequency(screenWidth, screenHeight);
	createRenderObjects(hWnd, screenWidth, screenHeight);
	createViewSpace(screenWidth, screenHeight);
}

Direct3D::~Direct3D()
{
	if (_rasterizerState != nullptr) {
		_rasterizerState->Release();
		_rasterizerState = nullptr;
	}

	if (_blendState != nullptr) {
		_blendState->Release();
		_blendState = nullptr;
	}

	if (_depthStencilState != nullptr) {
		_depthStencilState->Release();
		_depthStencilState = nullptr;
	}

	if (_renderTargetView != nullptr) {
		_renderTargetView->Release();
		_renderTargetView = nullptr;
	}

	if (_deviceContext != nullptr) {
		_deviceContext->Release();
		_deviceContext = nullptr;
	}

	if (_device != nullptr) {
		_device->Release();
		_device = nullptr;
	}

	if (_swapChain != nullptr) {
		_swapChain->Release();
		_swapChain = nullptr;
	}
}

void Direct3D::beginScene(FLOAT r1, FLOAT r2, FLOAT r3)
{
	FLOAT color[4] { r1, r2, r3, 1.0f };

	_deviceContext->ClearRenderTargetView(_renderTargetView, color);
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::endScene()
{
	_swapChain->Present(1, 0);
}

void Direct3D::getOutputFrequency(UINT screenWidth, UINT screenHeight)
{
	IDXGIFactory* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	if (FAILED(hr)) {
		throw new std::exception("Could not create DXGI factory. Direct3D.cpp:getOutputFrequency");
	}

	IDXGIAdapter* adapter = nullptr;
	hr = factory->EnumAdapters(0, &adapter);

	if (FAILED(hr)) {
		throw new std::exception("Could not find DXGI Enum Adapters. Direct3D.cpp:getOutputFrequency");
	}

	IDXGIOutput* output = nullptr;
	hr = adapter->EnumOutputs(0, &output);

	if (FAILED(hr)) {
		throw new std::exception("Could not find DXGI Enum Outputs. Direct3D.cpp:getOutputFrequency");
	}

	UINT modeCount = 0;
	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &modeCount, nullptr);

	if (FAILED(hr) || modeCount == 0) {
		throw new std::exception("Could not get display mode description count. Direct3D.cpp:getOutputFrequency");
	}

	DXGI_MODE_DESC* descriptions = new DXGI_MODE_DESC[modeCount];
	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &modeCount, descriptions);

	if (FAILED(hr)) {
		throw new std::exception("Could not get display mode description. Direct3D.cpp:getOutputFrequency");
	}

	for (UINT i = 0; i < modeCount; ++i) {
		if (descriptions[i].Width == screenWidth && descriptions[i].Height == screenHeight) {
			_refreshRate = descriptions[i].RefreshRate;
			break;
		}
	}

	delete[] descriptions;
	descriptions = nullptr;

	output->Release();
	output = nullptr;

	factory->Release();
	factory = nullptr;
}

void Direct3D::createRenderObjects(HWND hWnd, UINT screenWidth, UINT screenHeight)
{
	DXGI_SWAP_CHAIN_DESC swapChainDescription {};
	swapChainDescription.BufferCount = 1;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.Width = screenWidth;
	swapChainDescription.BufferDesc.Height = screenHeight;
	swapChainDescription.BufferDesc.RefreshRate = _refreshRate;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = hWnd;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Windowed = true;

	D3D_FEATURE_LEVEL levels[] { D3D_FEATURE_LEVEL_10_1 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, // graphic adapter or nullptr for primary one
		D3D_DRIVER_TYPE_HARDWARE, // complete rendering on gfx
		nullptr, // only for D3D_DRIVER_TYPE_SOFTWARE
		0, // no flags
		levels, 1,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&_swapChain,
		&_device,
		nullptr, // only necessary if you provide more than one feature level
		&_deviceContext
	);

	if (FAILED(hr)) {
		throw new std::exception("Could not create device and swap chain. Direct3D.cpp:createRenderObjects");
	}

	ID3D11Texture2D* texture = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));

	if (FAILED(hr)) {
		throw new std::exception("Could not get texture buffer. Direct3D.cpp:createRenderObjects");
	}

	hr = _device->CreateRenderTargetView(texture, nullptr, &_renderTargetView);

	if (FAILED(hr)) {
		throw new std::exception("Could not create render target view. Direct3D.cpp:createRenderObjects");
	}

	texture->Release();
	texture = nullptr;

	D3D11_TEXTURE2D_DESC depthStencilTexture {};
	depthStencilTexture.ArraySize = 1;
	depthStencilTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTexture.Width = screenWidth;
	depthStencilTexture.Height = screenHeight;
	depthStencilTexture.MipLevels = 1;
	depthStencilTexture.SampleDesc.Count = 1;

	hr = _device->CreateTexture2D(&depthStencilTexture, nullptr, &texture);

	if (FAILED(hr)) {
		throw new std::exception("Could not create 2d texture. Direct3D.cpp:createRenderObjects");
	}

	D3D11_RASTERIZER_DESC rasterizerDescription {};
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;

	hr = _device->CreateRasterizerState(&rasterizerDescription, &_rasterizerState);

	if (FAILED(hr)) {
		throw new std::exception("Could not create rasterizer state. Direct3D.cpp:createRenderObjects");
	}

	hr = _device->CreateDepthStencilView(texture, nullptr, &_depthStencilView);

	texture->Release();
	texture = nullptr;

	if (FAILED(hr)) {
		throw new std::exception("Could not create depth stencil view. Direct3D.cpp:createRenderObjects");
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	hr = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);

	if (FAILED(hr)) {
		throw new std::exception("Could not create depth stencil state. Direct3D.cpp:createRenderObjects");
	}

	D3D11_BLEND_DESC blendStateDesc {};
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = _device->CreateBlendState(&blendStateDesc, &_blendState);

	if (FAILED(hr)) {
		throw new std::exception("Could not create blend state. Direct3D.cpp:createRenderObjects");
	}

	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 0);
	_deviceContext->OMSetBlendState(_blendState, nullptr, 0xffffffff);
	_deviceContext->RSSetState(_rasterizerState);
}

void Direct3D::createViewSpace(UINT screenWidth, UINT screenHeight)
{
	D3D11_VIEWPORT viewport {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	_deviceContext->RSSetViewports(1, &viewport);
}
