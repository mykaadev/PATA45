#include "EngineTime.h"
#include "sdl.h"

EngineTime* EngineTime::m_Instance = nullptr;

void EngineTime::Tick()
{
	m_DeltaTime = (SDL_GetTicks() - m_LastTime) * (TARGET_FPS / 1000.0f);

	if (m_DeltaTime > TARGEET_DELTATIME)
	{
		m_DeltaTime = TARGEET_DELTATIME;
	}

	m_LastTime = SDL_GetTicks();
}

