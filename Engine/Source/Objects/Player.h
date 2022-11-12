#pragma once

#include "Character.h"

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
	
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();

private:
	
	void HandleInput();
	void SetOriginPoint();
	void SetAnimationState(AnimationStates inCurrentAnimationState, float inAxisValue);

	bool bIdle, bMovingRight, bMovingLeft;
	
	~Player();


};

