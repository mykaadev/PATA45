#pragma once
class PowerUpsSpawner
{
public:

	static PowerUpsSpawner* GetInstance() {
		return m_Instance
			= (m_Instance != nullptr) ? m_Instance : new PowerUpsSpawner();
	}
	static PowerUpsSpawner* m_Instance;

	PowerUpsSpawner() {};

	~PowerUpsSpawner();

	void Init();
};

