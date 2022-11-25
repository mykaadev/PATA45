#pragma once

#include "../Source/Objects/Character.h"
#include "BaseEnemy.h"


class LonerEnemy : public BaseEnemy
{
public:

	LonerEnemy(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckColision();

};

