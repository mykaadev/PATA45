#pragma once

#include <box2d.h>
#include "../Objects/Character.h"



enum AsteroidSpinning
{
	rotating
};

class BaseAsteroid: public Character
{

public:

	BaseAsteroid(Properties* props);

	
	enum Size {Big,Medium,Small};

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckColision(GameObject* otherGameObject);
	virtual void TakeDamage(int inDamage);
	virtual void GiveDamage(GameObject* otherGameObject);

	
	/*Classic stuff */
	void SetupBody();
	void SetAnimationState(AsteroidSpinning inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();
	
	/*To spawn the asteroid*/
	virtual void ChooseType();

	inline b2Body* GetBody() { return m_Body; }
	inline bool GetIsDead() { return m_IsDead; }


	bool m_IsDead;


protected:

	Size GetSize() { return size_; }
	Size size_;

};