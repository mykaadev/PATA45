#pragma once
#include <box2d.h>
#include <Character.h>


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

private:

	void SetupBody();
	void SetAnimationState(EnemyState inCurrentAnimationState, float inAxisValue);
	void SetOriginPoint();
	b2Body* m_Body;


};

