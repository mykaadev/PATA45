#pragma once
#include "WorldObject.h"
#include "../Graphics/Animation.h"
#include "box2d.h"


/*
*    IObject
*    |--GameObject
*    |----Character
*
*    Character Handles Animations
* 
*    TODO: Add RigidBody Component
*     
*
*/

class Character : public WorldObject
{
public:
	Character(Properties* props);

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Clean() = 0;
	virtual void CheckCollision(GameObject* otherGameObject) = 0;
	inline Animation* GetAnimation() { return m_Animation; }

protected:
	Animation* m_Animation;
};

