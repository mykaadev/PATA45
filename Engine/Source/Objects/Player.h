#pragma once

#include "Character.h"

class Player : public Character
{
public:
	Player(Properties* props);
	
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
};

