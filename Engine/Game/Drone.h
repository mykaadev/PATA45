#pragma once
#include "../Source/Objects/Character.h"

#include "BaseEnemy.h"
class Drone :public BaseEnemy{


public:

	LonerEnemy(Properties* props);

	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void TakeDamage(int inDamage) override;

	SDL_TimerID myTimerID;

	void SetOriginPoint();
	int m_CurrentHealth;
	int m_MaxHealth;
private:
	bool m_GoingRight;


};

