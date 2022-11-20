#pragma once
class ObjectHandler
{
public:

	static ObjectHandler* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new ObjectHandler(); }



	void LoadObjects();


private:

	ObjectHandler() {};
	~ObjectHandler() { delete m_Instance; };
	static ObjectHandler* m_Instance;

};

