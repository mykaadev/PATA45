#pragma once


class EnemySpawner
{
public:

	static EnemySpawner* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new EnemySpawner(); }
	static EnemySpawner* m_Instance;

	EnemySpawner();

	~EnemySpawner();

	void Init();

private:
	



};

