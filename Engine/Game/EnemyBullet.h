#pragma once
#include "../Source/Objects/Character.h"
#include "box2d.h"
#include "World.h"

class EnemyBullet : public Character
{
public:
	EnemyBullet(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckCollision(GameObject* otherGameObject);

	void SetupBody();
	void SetOriginPoint();

	int m_damageAmount;
	bool m_IsDead;
	~EnemyBullet();
};

