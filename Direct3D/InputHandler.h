#pragma once
#include <stdio.h>
#include <memory>
#include "InputListener.h"
#include <list>
#include <Windows.h>
#include "helpers.h"

class InputHandler
{
public:
	static InputHandler* instance()
	{
		static InputHandler instance;
		return &instance;
	};

	InputHandler();
	~InputHandler();

	void handle(char character);
	void listen(InputListener* observer);
	void unlisten(InputListener* observer);

private:
	std::list<InputListener*> _listeners;
};

