#pragma once

#include "../Source/Objects/Character.h"
#include "BasePowerUp.h"
class WeaponPowerUp :public BasePowerUp
{

public:

	WeaponPowerUp(Properties* props);
	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void OnPickUp(GameObject* otherGameObject);


	void SetOriginPoint();
	void ApplyPowerUp(int amount);
	

private:
	int bulletType;
	int m_FirePower;
	int m_CurrentHealth;
	int m_MaxHealth;

	

};

