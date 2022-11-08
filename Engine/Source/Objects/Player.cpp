#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"

Player::Player(Properties* props) : Character(props) {
	
	m_Animation = new Animation();

}

void Player::Draw()
{
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);

}


void Player::Update(float deltaTime)
{
	m_Animation->SetProperties("Player", 1, 1, 60, SDL_FLIP_NONE);


    if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_A))
    {
        m_Animation->SetProperties("Player", 1, 7, 60, SDL_FLIP_NONE);
    }

    if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_D))
    {
		m_Animation->SetProperties("Player", 1, 7, 60, SDL_FLIP_NONE);
    }
   
    m_Origin->X = m_Transform->X + m_Width / 2;
    m_Origin->Y = m_Transform->Y + m_Height / 2;


    m_Transform->Interp((m_Transform->X, m_Transform->Y), (m_Transform->X += 0.2f, m_Transform->Y -= 0.5f), 2.f);
    m_Animation->Update();
}

void Player::Clean()
{
    TextureManager::GetInstance()->Clean();
}
