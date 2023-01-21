#include "WeaponPowerUp.h"
#include "World.h"
#include "EngineTime.h"
#include "Bullet.h"
#include "Player.h"
#include <iostream>


Bullet* bullet = nullptr;

WeaponPowerUp::WeaponPowerUp(Properties* props) : BasePowerUp(props)
{
	m_CurrentHealth = 1;
	m_FirePower = 3;
	//starts at zero to choose the small bullet
	bulletType = 0;
}

void WeaponPowerUp::Init()
{
	__super::Init();

	m_Animation->SetProperties("WeaponPowerUp", 1, 0, 8, 100, true);

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Body->SetLinearVelocity(b2Vec2(0.f, 1.5f));
	}
}

void WeaponPowerUp::Draw()
{
	__super::Draw();
}

void WeaponPowerUp::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();

	if (m_Body->GetPosition().y > 700.0f && !m_IsDead && m_Body != nullptr)
	{
		m_IsDead = true;
		m_Height = m_Width;

		if (m_Body != nullptr && !World::GetInstance()->GetWorld()->IsLocked())
		{
			m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}
		

		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
		m_Animation->SetCurrentSprite(0);

	}

	if (m_IsDead)
	{
		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}

	m_Animation->Update(deltaTime);
}

void WeaponPowerUp::Clean()
{
	m_PendingKill = true;
}

void WeaponPowerUp::CheckCollision(GameObject* otherGameObject)
{

}

void WeaponPowerUp::OnPickUp(GameObject* otherGameObject)
{
	if (dynamic_cast<Player*>(otherGameObject)->bulletLevel < 2)
	{
		dynamic_cast<Player*>(otherGameObject)->ChooseBulletType();
		m_IsDead = true;
		Clean();
	}

}


void WeaponPowerUp::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
}

