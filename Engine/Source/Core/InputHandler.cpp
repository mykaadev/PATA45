#include "InputHandler.h"
#include "Engine.h"
#include <iostream>
#include "../Physics/Vector2.h"


InputHandler* InputHandler::m_Instance = nullptr;

InputHandler::InputHandler()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
	CheckForGamepadCompatibility();

}

void InputHandler::Listen()
{

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT: Engine::GetInstance()->Quit(); break;
			case SDL_KEYDOWN: KeyDown(); break;
			case SDL_KEYUP: KeyUp();  break;
			case SDL_JOYAXISMOTION:  
				if (event.jaxis.value < 0 || event.jaxis.value > 0)
				{
					std::cout << event.jaxis.value << std::endl;
				}


		}		
	}
}

bool InputHandler::GetKeyDown(SDL_Scancode inKey)
{
	if (m_KeyStates[inKey] == 1) 
	{
		return true;
	}
	
	return false;
}


void InputHandler::KeyUp()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void InputHandler::KeyDown()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}


void InputHandler::CheckForGamepadCompatibility()
{
	
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
	{
		std::cout << "ERROR: GamePad failed to initialize" << std::endl;
	}
	if (SDL_NumJoysticks() < 1)
	{
		std::cout << "Warning: No Gamepad detected" << std::endl;
	}
	else
	{
		m_Controller = SDL_GameControllerOpen(0);
	}

	SDL_Joystick* joystick = SDL_JoystickOpen(0);

	if (joystick == NULL)
	{
		std::cout << "Unable to use Gamepad: " << SDL_GetError() << std::endl;
	}
	else
	{
		std::cout << "Name: " << SDL_JoystickName(joystick) << std::endl;
		std::cout << "Num Axes: " << SDL_JoystickNumAxes(joystick) << std::endl;
		std::cout << "Num Buttons: " << SDL_JoystickNumButtons(joystick) << std::endl;
		std::cout << "Num Track balls: " << SDL_JoystickNumBalls(joystick) << std::endl;
		std::cout << "Num Hats: " << SDL_JoystickNumHats(joystick) << std::endl;
	}

	SDL_JoystickClose(joystick);
}

InputHandler::~InputHandler()
{
	delete m_KeyStates;
	delete m_Controller;
	delete m_Instance;
}

