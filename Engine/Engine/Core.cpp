#include "Core.h"
#include "../Source/Core/Engine.h"
#include "../Source/Core/Time.h"
#include "box2d.h"


#undef main;

int main()
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

	return 0;
}
