#pragma once

#include "../Objects/Character.h"
#include "box2d.h"

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
	void CheckColision();


private:

	void SetupBody();
	void SetAnimationState(BulletStates inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();
	b2Body* m_Body;

	~Bullet();
};