#pragma once
class AsteroidSpawner
{

public:
		
	enum AsteroidType{Stone, Metal};
	enum Size{Big,Medium,Small};


	static AsteroidSpawner* GetInstance() {
		return m_Instance
			= (m_Instance != nullptr) ? m_Instance : new AsteroidSpawner();
	}
	static AsteroidSpawner* m_Instance;


	AsteroidSpawner();
	~AsteroidSpawner();

	void Init();

private:

};

