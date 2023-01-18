#include "StoneAsteroid.h"
#include "World.h"
#include "EngineTime.h"
#include "Player.h"
#include <iostream>

StoneAsteroid::StoneAsteroid(Properties* props) : BaseAsteroid(props)
{
	r = 0;
	m_CurrentHealth = m_MaxHealth;

	m_GoingRight = true;
}

void StoneAsteroid::Init()
{
	__super::Init();

	SetupBody();

	m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);

	SpawnAsteroid();

}

void StoneAsteroid::Draw()
{
	__super::Draw();
}


void StoneAsteroid::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();



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

		TakeDamage(0);
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



void StoneAsteroid::TakeDamage(int damage)
{

	if (damage > 0)
	{

		m_CurrentHealth -= damage;

	}
	if (m_CurrentHealth<= 0 )
	{
		// if u got time in the morning see how to make the player life and all that 
		if (size_ == Big || size_ == Medium)
		{
			Split();
		}
		else 
		{
			Explosion();
		}
	}

}

/* Goes to the player and gives damage  */
void StoneAsteroid::GiveDamage()
{
}

/* follows the same logic of the metal Asteroid */
void StoneAsteroid::SpawnAsteroid()
{

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		int r = rand() % 3;

		switch (r) {
		case 0:
			size_ = Big;
			m_Animation->SetProperties("Stone", 1, 0, 25, 200, true);
			// it takes two bullets to kill this stone
			m_CurrentHealth = 2;
			break;
		case 1:
			size_ = Medium;
			m_Animation->SetProperties("StoneMedium", 1, 0, 24, 200, true);
			// it takes one bullet to kill this stone
			m_CurrentHealth = 1;
			break;
		case 2:
			size_ = Small;
			m_Animation->SetProperties("StoneSmall", 1, 0, 16, 200, true);
			// it takes 1/2 bullet to kill this stone because is the smallest 
			m_CurrentHealth = 0.5;
			break;

		}
	}
}


void StoneAsteroid::Split()
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		//create new asteroids
		StoneAsteroid* asteroid1 = new StoneAsteroid(new Properties("Asteroid1", m_Body->GetPosition().x - 30, m_Body->GetPosition().y - 30, 32, 32));
		StoneAsteroid* asteroid2 = new StoneAsteroid(new Properties("Asteroid2", m_Body->GetPosition().x + 30, m_Body->GetPosition().y - 30, 32, 32));
		StoneAsteroid* asteroid3 = new StoneAsteroid(new Properties("Asteroid3", m_Body->GetPosition().x, m_Body->GetPosition().y + 30, 32, 32));

		// set properties for each new asteroid
		if (size_ == Big)
		{
			asteroid1->m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);
			asteroid1->m_CurrentHealth = 1;
			asteroid2->m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);
			asteroid2->m_CurrentHealth = 1;
			asteroid3->m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);
			asteroid3->m_CurrentHealth = 1;
		}
		else if (size_ == Medium)
		{
			asteroid1->m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);
			asteroid1->m_CurrentHealth = 1;
			asteroid2->m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);
			asteroid2->m_CurrentHealth = 1;
			asteroid3->m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);
			asteroid3->m_CurrentHealth = 1;
		}

		//load the new asteroids into the world
		World::GetInstance()->LoadObjects(asteroid1);
		World::GetInstance()->LoadObjects(asteroid2);
		World::GetInstance()->LoadObjects(asteroid3);

		//destroy the original asteroid
		Clean();
	}
}


void StoneAsteroid::Explosion()
{
	m_IsDead = true;
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
	m_Animation->SetCurrentSprite(0);
	m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
}

void StoneAsteroid::Clean()
{
	EngineTime::GetInstance()->RemoveTimer(myTimerID);
	m_PendingKill = true;
}

void StoneAsteroid::CheckCollision(GameObject* otherGameObject)
{
}

/* Set the position */
void StoneAsteroid::SetOriginPoint()
{
	if (!World::GetInstance()->GetWorld()->IsLocked() && m_Body != nullptr)
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}

}

