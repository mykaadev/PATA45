#pragma once

#include "../Objects/Character.h"

class BaseAsteroid
{

public:

	BaseAsteroid(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckColision();
	virtual void TakeDamage(int inDamage);

	void SetupBody();
	void SetAnimationState(EnemyState inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();

	inline b2Body* GetBody() { return m_Body; }
	inline bool GetIsDead() { return m_IsDead; }

	bool m_IsDead;
};