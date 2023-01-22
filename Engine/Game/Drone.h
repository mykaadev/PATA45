#pragma once

#include "../Source/Objects/Character.h"
#include "BaseEnemy.h"


struct SineMovement
{
	float ticksCounter;
	float sineSpeed;
	float sineAmplitude;
};

class Drone : public BaseEnemy
{
public:
	Drone(Properties* props);

	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void TakeDamage(int inDamage) override;
	void SetOriginPoint();

	/* Drone Part */
	void Explosion();
	bool m_IsDead;

private:
	SineMovement m_SineMovement;

	//life
	int m_CurrentHealth;
	int m_MaxHealth;
	float xMovement;
	float yMovement;

	bool collided;
};


