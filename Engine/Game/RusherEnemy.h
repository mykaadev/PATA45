#pragma once
#include "BaseEnemy.h"

class RusherEnemy : public BaseEnemy
{
public:

	RusherEnemy(Properties* props);
	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void TakeDamage(int inDamage) override;

	bool m_GoingRight;
	


	int m_CurrentHealth;
	int m_MaxHealth;
	
	RusherEnemy();
};

