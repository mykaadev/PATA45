#include "MetalAsteroid.h"
#include "World.h"
#include "EngineTime.h"
#include <iostream>

MetalAsteroid::MetalAsteroid(Properties* props) : BaseAsteroid(props)
{
	b_IsIndestructible = true;
	r = 0;
	m_GoingRight = true;
	//m_Body->SetLinearVelocity(b2Vec2(0.f, 1.5f));

	
// 	switch (size)
// 	{
// 	case BaseAsteroid::Big:
// 		m_Animation->SetProperties("Metal", 1, 0, 25, 50, true, SDL_FLIP_NONE);
// 
// 		break;
// 	case BaseAsteroid::Medium:
// 		m_Animation->SetProperties("MetalMedium", 1, 0, 24, 50, true, SDL_FLIP_NONE);
// 
// 		break;
// 	case BaseAsteroid::Small:
// 		m_Animation->SetProperties("MetalSmall", 1, 0, 16, 50, true);
// 		break;
// 	default:
// 		break;
// 	}
	
}

/* Set the default asteroid for the small one */
void MetalAsteroid::Init()
{
	__super::Init();

	SetupBody();

	m_Animation->SetProperties("MetalSmall", 1, 0, 16, 100, true);
	SpawnAsteroid();
}

void MetalAsteroid::Draw()
{
	__super::Draw();
}

void MetalAsteroid::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		if (!m_IsDead && m_Body != nullptr)
		{
			if (m_GoingRight && m_Body->GetPosition().x < 960 - m_Width / 2)
			{
				m_Body->SetLinearVelocity(b2Vec2(2.0f, 1.0f));
			}

			if (m_GoingRight && m_Body->GetPosition().x >= 960 - m_Width / 2)
			{
				m_GoingRight = false;
			}

			if (!m_GoingRight && m_Body->GetPosition().x > 900 - m_Width / 2)
			{
				m_Body->SetLinearVelocity(b2Vec2(-2.0f, 1.0f));

			}

			if (!m_GoingRight && m_Body->GetPosition().x <= 0 + m_Width / 2)
			{
				m_GoingRight = true;
			}

			//Handle Out of screen destroy
			if (m_Body->GetPosition().y > 640.0f + m_Height / 2 && !m_IsDead)
			{
				m_IsDead = true;
				if (!World::GetInstance()->GetWorld()->IsLocked())
				{
					m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				}
				m_Animation->SetCurrentSprite(0);
				m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);

			}
		}
	}
	
}

void MetalAsteroid::Clean()
{
	EngineTime::GetInstance()->RemoveTimer(myTimerID);
	m_PendingKill = true;
}

void MetalAsteroid::CheckCollision(GameObject* otherGameObject)
{

}

/* Gives damage when colliding with the player*/
void MetalAsteroid::GiveDamage()
{
	/**
	 * Go to the player and them taking health
	 */
}


/**
 * Chose a random number between 0 and 3 and set the size for the spawning
 */
void MetalAsteroid::SpawnAsteroid()
{
	
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		int r = rand() % 3;

		switch (r) {
		case 0:
			size_ = Big;
			m_Animation->SetProperties("Metal", 1, 0, 25, 200, true);
			break;
		case 1:
			size_ = Medium;
			m_Animation->SetProperties("MetalMedium", 1, 0, 24, 200, true);
			break;
		case 2:
			size_ = Small;
			m_Animation->SetProperties("MetalSmall", 1, 0, 16, 200, true);
			break;

		}

		std::cout << "Chosen Metal Asteroid Value: " << r << std::endl;
	}
}

/* Set the position */
void MetalAsteroid::SetOriginPoint()
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
	
}
