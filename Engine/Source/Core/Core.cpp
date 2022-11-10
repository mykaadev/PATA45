#include "Core.h"
#include "Engine.h"
#include "EngineTime.h"
#include "box2d.h"
#include <iostream>


Core* Core::m_Instance = nullptr;


void Core::InitEngine()
{
 	Engine::GetInstance()->Init();
 
	while (Engine::GetInstance()->IsRunning())
	{
		Engine::GetInstance()->Events();
		Engine::GetInstance()->Update();
		Engine::GetInstance()->Renders();
		EngineTime::GetInstance()->Tick();
	}

	Engine::GetInstance()->Clean();

}

void Core::Init()
{
	std::cout << " yoh " << std::endl;
}
