#pragma once
#include "box2d.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Level.h"
#include "..\Objects\GameObject.h"

class World : public b2ContactListener

{
public:

	static World* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new World(); }

	void SetupWorld();
	void HandlePhysics(float deltaTime);

	void Update(float deltaTime);
	void Render();

	inline b2World* GetWorld() {return m_World.get();}

	inline void LoadObjects(GameObject* Object) { GameObjectLoaded.push_back((GameObject*)Object); Object->Init(); }


 	//void DestroyGameObject(GameObject* inObject, b2Body* body = nullptr);
	inline void NewBodyPendingKill(b2Body* body) { BodiesPendingKill.push_back(body); }
	void CleanPendingKills();

	void BeginContact(b2Contact* contact);

private:

	static World* m_Instance;
	std::unique_ptr<b2World> m_World;
	std::vector <GameObject*> GameObjectLoaded;
	std::vector <b2Body*> BodiesPendingKill;


	Level* m_Level;

	World();
	~World();


	void DestroyPendingKillObjects();
	void GatherPendingObjects();
};
