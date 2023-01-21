#pragma once

#include "../Objects/Character.h"
#include "box2d.h"
#include "World.h"


class Bullet : public Character
{
public:

	enum{light, medium, hard};
	Bullet(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckCollision(GameObject* otherGameObject);

	inline void SetDamage(int newDamage) { m_damageAmount = newDamage;};
	
private:
	void SetupBody();
	void SetOriginPoint();
	void CheckIfIsOutOfBounds();
	int m_damageAmount;

public:

	void ChooseType(int type);
	
	bool m_IsDead;
	~Bullet();
};

