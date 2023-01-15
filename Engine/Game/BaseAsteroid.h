#pragma once

#include <box2d.h>
#include "../Objects/Character.h"



class BaseAsteroid: public Character
{

public:

	BaseAsteroid(Properties* props);

	enum Size {Big,Medium,Small};

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckColision();
	virtual void TakeDamage(int inDamage);
	virtual void GiveDamage();

	

	void SetupBody();
	void SetAnimationState(BaseAsteroid inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();
	void Split();

	inline b2Body* GetBody() { return m_Body; }
	inline bool GetIsDead() { return m_IsDead; }


	bool m_IsDead;
	Size GetSize() { return size_; }
	int health_ = 100;

protected:

	Size GetSize() { return size_; }
	Size size_;

};