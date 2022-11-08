#include "Engine.h"
#include <iostream>
#include "sdl.h"
#include "../Graphics/TextureManager.h"
#include "../Objects/Player.h"
#include "../Core/InputHandler.h"
#include "Time.h"
#include "LevelParser.h"
#include <iostream>
#include "../Components/Camera.h"

Engine* Engine::m_Instance = nullptr;

Player* player = nullptr;

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
		std::cout << "Failed to load map" << std::endl;
	}

	m_Level = LevelParser::GetInstance()->GetLevel("Level0");


	TextureManager::GetInstance()->Load("Logo", "../Assets/EngineLogo.bmp");
	TextureManager::GetInstance()->Load("Player", "../Assets/PlayerTestMovement.bmp");
	player = new Player(new Properties("Player", SCREEN_WIDTH/2, SCREEN_HEIGHT, 64, 64, SDL_FLIP_NONE));

	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());

	return m_bIsRunning = true;
}


void Engine::Events()
{
	InputHandler::GetInstance()->Listen();
}


void Engine::Update()
{
	float deltaTime = Time::GetInstance()->GetDeltaTime();

	player->Update(deltaTime);
	Camera::GetInstance()->Update(deltaTime);
	m_Level->Update();
}

void Engine::Renders()
{
	SDL_SetRenderDrawColor(m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(m_Renderer);

	m_Level->Render();
	player->Draw();

	TextureManager::GetInstance()->Draw("Logo", 0, 0, 960, 640, SDL_FLIP_NONE);
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

	SDL_Log("Texture Maps Cleaned");
	return true;
}


Engine::~Engine()
{
	delete m_Renderer;
	delete m_Window;
	delete m_Instance;
}