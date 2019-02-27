#pragma once
#include <Windows.h>
#include <dxgi.h>
#include <d3d11.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

class Direct3D
{
public:
	Direct3D(HWND hWnd, UINT screenWidth, UINT screenHeight);
	~Direct3D();

	void beginScene(FLOAT r1, FLOAT r2, FLOAT r3);
	void endScene();

	ID3D11Device* getDevice()
	{
		return _device;
	}

	ID3D11DeviceContext* getDeviceContext()
	{
		return _deviceContext;
	}

private:
	DXGI_RATIONAL _refreshRate {};
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;
	IDXGISwapChain* _swapChain = nullptr;
	ID3D11RenderTargetView* _renderTargetView = nullptr;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11DepthStencilState* _depthStencilState = nullptr;
	ID3D11BlendState* _blendState = nullptr;
	ID3D11RasterizerState* _rasterizerState = nullptr;

	void getOutputFrequency(UINT screenWidth, UINT screenHeight);
	void createRenderObjects(HWND hWnd, UINT screenWidth, UINT screenHeight);
	void createViewSpace(UINT screenWidth, UINT screenHeight);
};
