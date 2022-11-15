#pragma once

#include "SDL.h"
#include "LevelParser.h"
#include "../Objects/GameObject.h"
#include "../Objects/WorldObject.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

// Base class and add to an array 
// base level 
class Engine
{

public:
	static Engine* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Engine(); }
	
	bool Init();
	void Events();
	void Update();
	void Renders();
	bool Clean();
	void Quit();

	inline void LoadObjects(GameObject* Object) { GameObjectLoaded.push_back( (GameObject*) Object); }

	std::vector <GameObject*> GameObjectLoaded;

	inline bool IsRunning() { return m_bIsRunning; }
	inline SDL_Renderer* GetRenderer() { return m_Renderer; }


private:
	
	Engine();;
	~Engine();


	Level* m_Level;
	bool m_bIsRunning;
	static Engine* m_Instance;

	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;

};

