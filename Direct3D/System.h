#pragma once
#include "Graphics.h"
#include <Windows.h>
#include <chrono>
#include <unordered_set>
#include "UpdateTarget.h"

using namespace std::chrono;

class System
{
public:
	System(UINT width, UINT height);
	~System();

	void run();

	Graphics* getGraphics()
	{
		return _gfx;
	}

	void addUpdateTarget(UpdateTarget* target)
	{
		_updateTargets.insert(target);
	}

private:
	HWND _hWnd = nullptr;
	UINT _width = 0;
	UINT _height = 0;

	time_point<high_resolution_clock> _lastTime {};
	FLOAT _deltaTime = 0.0f;

	Graphics* _gfx = nullptr;
	std::unordered_set<UpdateTarget*> _updateTargets;

	void createWindow();
};
