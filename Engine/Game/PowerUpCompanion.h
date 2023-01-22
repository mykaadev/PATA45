#pragma once

#include "../Source/Objects/Character.h"

class PowerUpCompanion :public Character
{

public:
	bool collided;

	PowerUpCompanion(Properties* props);
	void Init() override;
	void SetupBody();
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void OnPickUp(GameObject* otherGameObject);

	void SetOriginPoint();
};

