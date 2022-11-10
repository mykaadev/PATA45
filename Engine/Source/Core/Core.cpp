#include "Core.h"
#include "Engine.h"
#include "EngineTime.h"
#include "box2d.h"
#include "Time.h"
#include "CodingHelper.h"

Core* Core::m_Instance = nullptr;


Core::Core()
{
	m_Instance = nullptr;
	CodingHelper::GetInstance()->IncrementAmountToClearCounter(1);
}

Core::~Core()
{
	delete m_Instance;
	CodingHelper::GetInstance()->DecrementAmountToClearCounter(1);
}

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
