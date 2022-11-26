#pragma once
#include "BaseEnemy.h"

class RusherEnemy : public BaseEnemy
{
public:

	RusherEnemy(Properties* props);
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();

	virtual void CheckColision();

	bool m_GoingRight;
	RusherEnemy();
};

