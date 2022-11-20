#include "Character.h"
#include "SDL.h"
#include "../Graphics/TextureManager.h"
#include "WorldObject.h"
#include "../Core/InputHandler.h"


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

Character::Character(Properties* props) : WorldObject(props)
{
    m_Animation = new Animation();
}


void Character::Draw()
{
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Character::Update(float deltaTime)
{
    m_Animation->Update(deltaTime);
}

void Character::Clean()
{
    TextureManager::GetInstance()->Clean();
}
