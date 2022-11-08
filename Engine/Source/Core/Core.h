#pragma once
class Core
{
public:
	static Core* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Core(); }

	void InitEngine();

private:

	Core() {}
	static Core* m_Instance;

};

