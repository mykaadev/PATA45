#pragma once

#include "../Objects/Character.h"
#include "Bullet.h"
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

	virtual void Init();
	virtual void Draw(); 
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckCollision(GameObject* otherGameObject);

	void TakeDamage(int inDamage);

private:

	void SetupBody();
	void BindAxisAndActions();
	void SetOriginPoint();
	void SetAnimationState(AnimationStates inCurrentAnimationState, float inAxisValue);
	void Move();
public:
	void FireGun();
private:
	SDL_TimerID myTimerID;

	bool bIdle, bMovingRight, bMovingLeft, canShoot, isDead;
	float fSpeed;
	int maxHealth = 100;
	int currentHealth = 100;
	

	Vector2 m_MoveAxis;

	~Player();
};

