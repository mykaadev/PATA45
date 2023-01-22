#pragma once

#include "../Source/Objects/Character.h"
#include "BasePowerUp.h"

class ShieldPowerUp :public BasePowerUp
{
public:

	ShieldPowerUp(Properties* props);
	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void TakeDamage(int damage) override;


	void SetOriginPoint();
	void ApplyPowerUp(GameObject* otherGameObject);



private:

	int addLife;
	int m_FirePower;
	int m_CurrentHealth;
	int m_MaxHealth;
};

