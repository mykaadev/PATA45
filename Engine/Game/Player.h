#pragma once

#include "../Objects/Character.h"


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

private:

	void SetupBody();
	void HandleInput();
	void SetOriginPoint();
	void SetAnimationState(AnimationStates inCurrentAnimationState, float inAxisValue);
	void Shooting();

	bool bIdle, bMovingRight, bMovingLeft;
	
	~Player();


};

