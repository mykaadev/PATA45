#include "Core.h"
#include "Engine.h"
#include "Time.h"
#include "box2d.h"


#undef main;


Core* Core::m_Instance = nullptr;


void InitEngine()
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
