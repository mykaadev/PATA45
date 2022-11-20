#pragma once
class Core
{
public:
	static Core* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Core(); }

	//Initializes the Engine Loop
	void InitEngine();

private:

	Core();

	~Core();

	static Core* m_Instance;


};

