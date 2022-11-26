#include "Engine.h"
#include <iostream>
#include "SDL.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"
#include "EngineTime.h"
#include "LevelParser.h"
#include <iostream>
#include "../Components/Camera.h"
#include "../../Game/ObjectHandler.h"
#include "../../Game/Player.h"
#include "World.h"

Engine* Engine::m_Instance = nullptr;


Engine::Engine()
{
	m_Instance = nullptr;
	m_Window = nullptr;
	m_Renderer = nullptr;

	m_bIsRunning = false;
}

Engine::~Engine()
{
	delete m_Renderer;
	delete m_Window;
	delete m_Instance;
}

bool Engine::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Failed to Initialize SDL: %s", SDL_GetError());
		return m_bIsRunning = false;
	}


	m_Window = SDL_CreateWindow("El Engenho Irreal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	
	if (m_Window == nullptr)
	{
		SDL_Log("Failed to Create Window: %s", SDL_GetError());
		return m_bIsRunning = false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	if (m_Renderer == nullptr)
	{
		SDL_Log("Failed to Create Renderer: %s", SDL_GetError());
		return m_bIsRunning = false;
	}
	

	if (!LevelParser::GetInstance()->Load())
	{
		std::cout<< "Failed to load map" << std::endl;
		return m_bIsRunning = false;
	}

	TextureManager::GetInstance()->ParseTextures("../Assets/Game/TextureParser.tml");

	return m_bIsRunning = true;
}


void Engine::Events()
{
	InputHandler::GetInstance()->Listen();
}


void Engine::Update()
{
	float deltaTime = EngineTime::GetInstance()->GetDeltaTime();
	World::GetInstance()->Update(deltaTime);
}

void Engine::Renders()
{

	SDL_SetRenderDrawColor(m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(m_Renderer);

	TextureManager::GetInstance()->Draw("EngineLogo", 0, 0, 960, 640, 1, 1, 0.5f);

	World::GetInstance()->Render();

	SDL_RenderPresent(m_Renderer);
}


void Engine::Quit()
{
	m_bIsRunning = false;
}

bool Engine::Clean()
{
	TextureManager::GetInstance()->Clean();
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();

	std::cout << "Texture Map Cleaned" << std::endl;

	return true;
}
