#pragma once

#include "../Objects/Character.h"
#include "box2d.h"
#include "World.h"

class Player;



class Companion : public Character
{
public:

	Companion(Properties* props);

	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void Clean();
	virtual void CheckCollision(GameObject* otherGameObject);

	Player* myPlayer;

	void SetPlayer(Player* inMyPlayer);

	void TakeDamage(int inDamage);

	inline bool GetIsDead() { return m_IsDead; }

public:
	void SetupBody();
	void SetOriginPoint();
	b2Vec2 vDesiredPosition;
	int m_damageAmount;

public:
	void SetPosition(b2Vec2 inPlayerPosition, float interpSpeed);

	bool m_IsDead;
	~Companion();
private:
	int currentHealth;
};

