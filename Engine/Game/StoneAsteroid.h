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
	void ChooseType();
	void Explosion();


	void SetOriginPoint();
	SDL_TimerID myTimerID;

	//life
	int m_DamageAmount;
	int m_CurrentHealth;
	int m_MaxHealth;
	int r;

	bool m_HasBeenSplit;

protected:
	Size GetSize() { return size_; }
	
private:
	bool collided;
};

