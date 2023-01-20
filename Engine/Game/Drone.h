#pragma once

#include "../Source/Objects/Character.h"
#include "BaseEnemy.h"



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

	float amplitude;
	float frequency;

	bool m_IsDead;

private:
	// float to keep track of the sinusoidal movement
	float m_SinCounter;
	//speed
	float m_Speed;
	// amplitude
	float m_Amplitude;
	float x;
	float y;
	//life
	int m_CurrentHealth;
	int m_MaxHealth;


};

