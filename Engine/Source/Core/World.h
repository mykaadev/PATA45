#pragma once
#include "box2d.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Level.h"
#include "..\Objects\GameObject.h"

class World
{
public:

	static World* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new World(); }

	void SetupWorld();
	void HandlePhysics(float deltaTime);

	void Update(float deltaTime);
	void Render();

	inline b2World* GetWorld() {return m_World.get();}

	inline void LoadObjects(GameObject* Object) { GameObjectLoaded.push_back((GameObject*)Object); Object->Init(); }

	inline void Destroy(b2Body* body) { m_World->DestroyBody(body);}

private:

	static World* m_Instance;
	std::unique_ptr<b2World> m_World;
	std::vector <GameObject*> GameObjectLoaded;
	Level* m_Level;

	World();
	~World();


};
