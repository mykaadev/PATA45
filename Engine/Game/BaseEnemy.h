#pragma once
#include <box2d.h>
#include "../Objects/Character.h"



enum EnemyState {
	attacking,
	dead
};
class BaseEnemy : public Character
{

public:

	BaseEnemy(Properties* props);

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


	
};

