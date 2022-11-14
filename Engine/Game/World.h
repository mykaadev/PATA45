#pragma once
#include "box2d.h"

class World
{
public:

	static World* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new World(); }

	void SetupWorld();
	void HandlePhysics(float deltaTime);
	inline b2World GetWorld() {	return myWorld;	}

private:
	World();

	static World* m_Instance;
	b2World myWorld;

};
