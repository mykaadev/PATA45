#pragma once

#include "../Source/Objects/Character.h"
#include "BaseAsteroid.h"
class StoneAsteroid : public BaseAsteroid {

public:
	StoneAsteroid(Properties* props);
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
	void Explosion();

	SDL_TimerID myTimerID;

	void SetOriginPoint();
	
	//life
	int m_CurrentHealth;
	int m_MaxHealth;
	int r;

	bool m_GoingRight;

protected:
	Size GetSize() { return size_; }
};

