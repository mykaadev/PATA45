#include "Core.h"
#include "Engine.h"
#include "EngineTime.h"
#include "box2d.h"
#include "Time.h"
#include "World.h"

Core* Core::m_Instance = nullptr;

Core::Core()
{
	m_Instance = nullptr;
}

Core::~Core()
{
	delete m_Instance;
}

void Core::InitEngine()
{
 	Engine::GetInstance()->Init();
	World::GetInstance()->SetupWorld();
 
	while (Engine::GetInstance()->IsRunning())
	{
		Engine::GetInstance()->Events();
		Engine::GetInstance()->Update();
		Engine::GetInstance()->Renders();
		EngineTime::GetInstance()->Tick();
	}

	Engine::GetInstance()->Clean();
}
