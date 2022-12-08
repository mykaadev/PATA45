#pragma once
class ObjectInitializer
{
public:

	static ObjectInitializer* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new ObjectInitializer(); }



	void LoadObjects();


private:

	ObjectInitializer() {};
	~ObjectInitializer() { delete m_Instance; };
	static ObjectInitializer* m_Instance;

};

