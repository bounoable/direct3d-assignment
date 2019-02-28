#pragma once
#include <d3d11.h>

class UpdateTarget
{
public:
	virtual void update(FLOAT deltaTime) = 0;
};