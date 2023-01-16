#pragma once

#include "../Source/Objects/Character.h"
#include "BaseAsteroid.h"


class MetalAsteroid : public BaseAsteroid
{
public:
	MetalAsteroid(Properties* props, Size size);
	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;


	/* Base Asteroid Part */
	void GiveDamage();
	void SpawnAsteroid();

	SDL_TimerID myTimerID;

	void SetOriginPoint();
	bool b_IsIndestructible;
	bool m_GoingRight;
	int r;

protected:
	Size GetSize() { return size_; }
};

