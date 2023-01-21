#pragma once
#include <box2d.h>
#include "../Objects/Character.h"


enum PowerupState
{
	spinning
};


class BasePowerUp : public Character {


public:

	BasePowerUp(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckColision();
	virtual void TakeDamage(int inDamage);

	/*Classic stuff */
	void SetupBody();
	void SetAnimationState(PowerupState inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();

	/*To spawn the power ups*/
	virtual void ChooseType();

	inline b2Body* GetBody() { return m_Body; }
	inline bool GetIsDead() { return m_IsDead; }

	bool m_IsDead;



};

