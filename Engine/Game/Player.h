#pragma once

#include "../Objects/Character.h"
#include "Bullet.h"
#include "HealthSystem.h"
#include "EnemyBullet.h"
#include "box2d.h"
#include <vector>

// custom player game side 

enum AnimationStates
{
	Idle,
	MovingX,
	MovingY,
	Dead
};


class Player : public Character
{
public:

	Player(Properties* props);
	Bullet* bullet;
	HealthSystem* health;
	EnemyBullet* enemeyBullet;

	virtual void Init();
	virtual void Draw(); 
	virtual void Update(float deltaTime);
	virtual void Clean();

private:

	void SetupBody();
	void HandleInput();
	void SetOriginPoint();
	void SetAnimationState(AnimationStates inCurrentAnimationState, float inAxisValue);
	void Shooting();
	void HealthHandler(int damage);
	void DeathAnimation();

	bool bIdle, bMovingRight, bMovingLeft, canShoot;
	float fSpeed;
	int maxHealth = 100;
	int currentHealth = 100;
	


	b2Body* m_Body;
	std::vector <GameObject*> myBullets;

	

	~Player();
};

