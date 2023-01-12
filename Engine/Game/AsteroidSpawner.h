#pragma once
class AsteroidSpawner
{

public:
		
	static AsteroidSpawner* GetInstance() {
		return m_Instance
			= (m_Instance != nullptr) ? m_Instance : new EnemySpawner();
	}
	static AsteroidSpawner* m_Instance;


	AsteroidSpawner();
	~AsteroidSpawner();

	void Init();

private:

};

