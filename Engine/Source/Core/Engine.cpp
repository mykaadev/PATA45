#include "Engine.h"
#include <iostream>
#include "SDL.h"
#include "../Graphics/TextureManager.h"
#include "../Objects/Player.h"
#include "../Core/InputHandler.h"
#include "EngineTime.h"
#include "LevelParser.h"
#include <iostream>
#include "../Components/Camera.h"
#include "CodingHelper.h"

Engine* Engine::m_Instance = nullptr;

Player* player = nullptr;

Engine::Engine()
{
	m_Instance = nullptr;
	m_Window = nullptr;
	m_Renderer = nullptr;
	m_bIsRunning = false;
	m_Level = nullptr;
	CodingHelper::GetInstance()->IncrementAmountToClearCounter(4);

}

Engine::~Engine()
{
	delete m_Renderer;
	delete m_Window;
	delete m_Instance;
	delete m_Level;
	CodingHelper::GetInstance()->DecrementAmountToClearCounter(4);

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
		CodingHelper::GetInstance()->DisplayInfo("Failed to load map");
		return m_bIsRunning = false;
	}

	m_Level = LevelParser::GetInstance()->GetLevel("Level0");

	TextureManager::GetInstance()->ParseTextures("../Assets/Game/TextureParser.tml");
	


	player = new Player(new Properties("Player", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 64, 64, SDL_FLIP_NONE));

	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());

	return m_bIsRunning = true;
}


void Engine::Events()
{
	InputHandler::GetInstance()->Listen();
}


void Engine::Update()
{
	float deltaTime = EngineTime::GetInstance()->GetDeltaTime();

	player->Update(deltaTime);
	Camera::GetInstance()->Update(deltaTime);
	m_Level->Update();
}

void Engine::Renders()
{

	SDL_SetRenderDrawColor(m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(m_Renderer);

	TextureManager::GetInstance()->Draw("EngineLogo", 0, 0, 960, 640, 1, 1, 0.5f);
	m_Level->Render();
	player->Draw();

	SDL_RenderPresent(m_Renderer);
}


void Engine::Quit()
{
	if (CodingHelper::GetInstance()->CanQuit())
	{
		m_bIsRunning = false;
	}
	else
	{
		CodingHelper::GetInstance()->DisplayPropertiesLeftToClear();
	}
}

bool Engine::Clean()
{
	TextureManager::GetInstance()->Clean();
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();

	CodingHelper::GetInstance()->DisplayInfo("Texture Maps Cleaned");

	return true;
}
