#pragma once

#include "../Source/Objects/Character.h"
#include "BaseAsteroid.h"
class StoneAsteroid : public BaseAsteroid {

public:
	StoneAsteroid(Properties* props, Size size);
	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void TakeDamage(int damage) override;

	/* Base Asteroid Part */
	void GiveDamage();
	void SpawnAsteroid();
	void Split();

	SDL_TimerID myTimerID;

	void SetOriginPoint();
	int m_CurrentHealth;
	int m_MaxHealh;
	bool m_GoingRight;
	int r;

protected:
	Size GetSize() { return size_; }
};

