
#include "sdl.h"
#include "../Physics/Vector2.h"

enum Axis {HORIZONTAL, VERTICAL};
class InputHandler
{

public:

	static InputHandler* GetInstance() {
		return m_Instance = (m_Instance != nullptr) ? m_Instance : new InputHandler();
	}

	//Listens for input events
	void Listen();

	//Listens for KeysPressed
	bool GetKeyDown(SDL_Scancode inKey);

	//Returns a vector from a default 2d axis
	Vector2 GetAxisKeys();
	float xValue;
	float yValue;
	

private:

	InputHandler();

	static InputHandler* m_Instance;

	void KeyUp();
	void KeyDown();

	void CheckForGamepadCompatibility();

	const Uint8* m_KeyStates;
	

};