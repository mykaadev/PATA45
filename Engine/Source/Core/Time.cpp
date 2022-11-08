#include "Time.h"
#include "sdl.h"

Time* Time::m_Instance = nullptr;

void Time::Tick()
{
	m_DeltaTime = (SDL_GetTicks() - m_LastTime) * (TARGET_FPS / 1000.0f);

	if (m_DeltaTime > TARGEET_DELTATIME)
	{
		m_DeltaTime = TARGEET_DELTATIME;
	}

	m_LastTime = SDL_GetTicks();
}

