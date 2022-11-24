#pragma once
#include "BaseEnemy.h"

class RusherEnemy : public BaseEnemy
{
	RusherEnemy();

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();

	void CheckColision();
};

