

const int TARGET_FPS = 60;
const float TARGEET_DELTATIME = 1.5f;


class Time
{
public:

	inline static Time* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Time(); }
	inline float GetDeltaTime() { return m_DeltaTime; }

	void Tick();


private:
	Time() {};

	static Time* m_Instance;

	float m_DeltaTime;
	float m_LastTime;

};

