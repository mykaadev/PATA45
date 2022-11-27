#pragma once

#include "../Objects/Character.h"
#include "box2d.h"
#include "World.h"

enum BulletStates
{
	travelling,
	explode
};


class Bullet : public Character
{
public:

	Bullet(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckCollision(GameObject* otherGameObject);

private:
	void SetupBody();
	void SetAnimationState(BulletStates inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();

	int m_damageAmount;
	b2Body* m_Body;

	~Bullet();
};

