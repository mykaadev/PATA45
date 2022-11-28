#include "SDL.h"
#include <vector>
#include <iostream>

const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 1.5f;
using TimerID = SDL_TimerID;
using T_Callback = SDL_TimerCallback;

class EngineTime
{
public:

	inline static EngineTime* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new EngineTime(); }
	inline float GetDeltaTime() { return m_DeltaTime; }

	inline void Clean() {
		for (TimerID id : m_TimerRegistry)
		{
			SDL_RemoveTimer(id);
		}
	}

	inline TimerID StartTimer(Uint32 interval, T_Callback callback, void* data)
	{
		TimerID id = SDL_AddTimer(interval, callback, data);
		m_TimerRegistry.push_back(id);
		return id;
	}

	inline void RemoveTimer(TimerID id)
	{
		SDL_RemoveTimer(id);
	}

	void Tick();

	std::vector<TimerID> m_TimerRegistry;

private:
	EngineTime() {};
	~EngineTime();


	static EngineTime* m_Instance;

	float m_DeltaTime;
	float m_LastTime;

};

