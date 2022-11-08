
#include "sdl.h"

class InputHandler
{

public:

	static InputHandler* GetInstance() {
		return m_Instance = (m_Instance != nullptr) ? m_Instance : new InputHandler();
	}

	void Listen();
	bool GetKeyDown(SDL_Scancode inKey);

private:

	InputHandler();

	static InputHandler* m_Instance;

	void KeyUp();
	void KeyDown();

	void CheckForGamepadCompatibility();

	const Uint8* m_KeyStates;
	

};