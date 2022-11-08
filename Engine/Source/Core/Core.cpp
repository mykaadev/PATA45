#include "Core.h"
#include "Engine.h"
#include "Time.h"


Core* Core::m_Instance = nullptr;

void Core::InitEngine()
{
 	Engine::GetInstance()->Init();
 
	while (Engine::GetInstance()->IsRunning())
	{
		Engine::GetInstance()->Events();
		Engine::GetInstance()->Update();
		Engine::GetInstance()->Renders();
		Time::GetInstance()->Tick();
	}

	Engine::GetInstance()->Clean();

}
