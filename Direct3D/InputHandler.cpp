#include "InputHandler.h"
#include <memory>
#include "helpers.h"


InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::handle(char character)
{
	for (const auto listener : _listeners) {
		listener->notifyInput(character);
	}
}

void InputHandler::listen(InputListener* observer)
{
	_listeners.push_back(observer);
}

void InputHandler::unlisten(InputListener* observer)
{
	_listeners.remove(observer);
}
