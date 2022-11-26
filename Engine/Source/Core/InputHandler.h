
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

private:

	InputHandler();

	static InputHandler* m_Instance;

	void KeyUp();
	void KeyDown();
	void GetAxis();


	void CheckForGamepadCompatibility();
	SDL_GameController* m_Controller;
	const Uint8* m_KeyStates;

	~InputHandler();
	

};