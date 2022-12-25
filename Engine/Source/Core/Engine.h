#pragma once

#include "SDL.h"
#include "LevelParser.h"
#include "../Objects/GameObject.h"
#include "../Objects/WorldObject.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640


class Engine
{

public:
	static Engine* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Engine(); }
	
	//Initializes the Window
	bool Init();

	//Handles inputs
	void Events();

	//Update Objects
	void Update();

	//Renders the Updated Objects
	void Renders();

	//Clean the engine
	bool Clean();

	//Closes the engine
	void Quit();



	inline bool IsRunning() { return m_bIsRunning; }
	inline SDL_Renderer* GetRenderer() { return m_Renderer; }
	inline bool UseLegacyRenderer() { return useLegacyRenderer;}


private:
	
	Engine();;
	~Engine();

	bool useLegacyRenderer;
	bool m_bIsRunning;
	static Engine* m_Instance;

	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_GLContext m_GLContext;



};

