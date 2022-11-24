#pragma once
#include "GameObject.h"
#include <string>


// as an transform and name 
// second level 
class WorldObject : public GameObject
{
public:
	WorldObject(Properties *props) : GameObject(props) {}

    
	virtual void Init() { __super::Init(); };
	virtual void Draw() = 0;
	virtual void Update(float deltaTime) { __super::Update(deltaTime); };
	virtual void Clean() = 0;

protected:
	std::string m_Name;
};

