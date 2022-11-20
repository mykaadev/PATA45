

const int TARGET_FPS = 60;
const float TARGEET_DELTATIME = 1.5f;


class EngineTime
{
public:

	inline static EngineTime* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new EngineTime(); }
	inline float GetDeltaTime() { return m_DeltaTime; }

	void Tick();


private:
	EngineTime() {};
	~EngineTime();


	static EngineTime* m_Instance;

	float m_DeltaTime;
	float m_LastTime;

};

