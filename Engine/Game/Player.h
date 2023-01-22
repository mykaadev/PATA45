#pragma once

#include "../Objects/Character.h"
#include "Bullet.h"
#include "box2d.h"
#include <vector>
#include "Companion.h"

class Companion;

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

	inline bool GetIsDead() { return m_IsDead; }
	void ChooseBulletType();
	void TakeDamage(int inDamage);
	void AddCompanion();

	/* PowerUps  */
	void AddSheildPowerUp(int morelife);
	void AddWeaponPowerUp(int Power);


private:

	void SetupBody();
	void BindAxisAndActions();
	void SetOriginPoint();
	void SetAnimationState(AnimationStates inCurrentAnimationState, float inAxisValue);
	void Move();

private:

public:
	void FireGun();
	int bulletLevel;
	int m_MoreLife;
	void SpawnCompanion();

public:
	SDL_TimerID myTimerID;
	SDL_TimerID myTimerIDCompanion;
	bool m_IsDead;

private:
	bool firstCompanionSpawned;

	bool bIdle, bMovingRight, bMovingLeft, canShoot;
	float fSpeed;
	int maxHealth = 100;
	int currentHealth = 100;
	int m_PowerLevel;
	int m_MaxPowerLevel;

	Companion* FirstCompanion;
	b2Vec2 firstCompanionPosition;
	Companion* SecondCompanion;
	b2Vec2 secondCompanionPosition;
	float fireRate;

public:
	void DetachCompanion(Companion* CompanionToCheck);
private:
	int m_DamageAmount;
	
	Vector2 m_MoveAxis;

	~Player();
	bool secondCompanionSpawned;
};

