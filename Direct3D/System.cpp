#include <exception>
#include <sstream>
#include "System.h"
#include <memory>
#include "InputHandler.h"


LRESULT CALLBACK WndProc(
	HWND hWnd, // handle to window the message is for
	UINT msg, // message id
	WPARAM wParam, // first parameter of message
	LPARAM lParam // second parameter of message
) {
	std::wstringstream ss;

	switch (msg) {
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
			return 0;
		}

		ss << wParam;

		// OutputDebugString(ss.str().c_str());

		InputHandler::instance()->handle(wParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

System::System(UINT width, UINT height)
{
	_width = width;
	_height = height;

	_lastTime = high_resolution_clock::now();
	
	createWindow();

	_gfx = new Graphics(_hWnd, _width, _height);
	addUpdateTarget(_gfx);
}


System::~System()
{
	if (_hWnd != nullptr) {
		DestroyWindow(_hWnd);
	}

	delete _gfx;
	_gfx = nullptr;
}

void System::run()
{
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	SetFocus(_hWnd);
	
	MSG msg {};
	time_point<high_resolution_clock> now {};
	
	FLOAT time = 0.0f;
	UINT fps = 0;

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		now = high_resolution_clock::now();
		_deltaTime = (now - _lastTime).count() * 0.000000001f;
		_lastTime = now;

		fps++;
		time += _deltaTime;

		if (time >= 1.0f) {
			std::wstringstream ss;
			ss << fps << std::endl;

			// OutputDebugString(ss.str().c_str());

			fps = 0;
			time = 0.0f;
		}

		for (const auto& target : _updateTargets) {
			target->update(_deltaTime);
		}
	}
}

void System::createWindow()
{
	// 1. create a window class structure
	WNDCLASS wc {};
	wc.hInstance = GetModuleHandle(nullptr);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	// wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.lpszClassName = TEXT("Direct 3D");
	wc.lpfnWndProc = WndProc;

	// 2. register window class
	if (!RegisterClass(&wc)) {
		throw new std::exception("Window class could not be registered. (System.cpp:createWindow)");
	}

	// 3. calculate window size and position (optional)
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT rect {};
	rect.left = (screenWidth - _width) / 2;
	rect.top = (screenHeight - _height) / 2;
	rect.right = rect.left + _width;
	rect.bottom = rect.top + _height;

	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rect, style, false);

	// 4. create a window instance
	_hWnd = CreateWindow(
		wc.lpszClassName, // class name of window to initialize
		wc.lpszClassName, // window title
		style, // visual style of the window
		rect.left, rect.top, // position of the left-top corner
		rect.right - rect.left, rect.bottom - rect.top, // width and height of the window
		nullptr, // handle to parent window (optional)
		nullptr, // handle to menu class (optional)
		wc.hInstance, // handle to application instance
		nullptr // additional parameters (optional)
	);

	if (!_hWnd) {
		throw new std::exception("Window instance could not be created. (System.cpp:createWindow)");
	}
}
