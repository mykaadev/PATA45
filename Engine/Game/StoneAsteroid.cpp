#include "StoneAsteroid.h"
#include "World.h"
#include "EngineTime.h"
#include "Player.h"
#include <iostream>

StoneAsteroid::StoneAsteroid(Properties* props) : BaseAsteroid(props)
{
	r = 0;
	m_CurrentHealth = m_MaxHealh;

	m_GoingRight = true;
	b_IsDead = false;
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

	m_Animation->Update(deltaTime);


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



void StoneAsteroid::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;

	if (m_CurrentHealth<= 0 )
	{
		// if u got time in the morning see how to make the player life and all that 
		if (size_ == Big|| size_ == Medium)
		{
			Split();
		}
		else {
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
			m_MaxHealh = 2;
			break;
		case 1:
			size_ = Medium;
			m_Animation->SetProperties("StoneMedium", 1, 0, 24, 200, true);
			// it takes one bullet to kill this stone
			m_MaxHealh = 1;
			break;
		case 2:
			size_ = Small;
			m_Animation->SetProperties("StoneSmall", 1, 0, 16, 200, true);
			// it takes 1/2 bullet to kill this stone because is the smallest 
			m_MaxHealh = 0.5;
			break;

		}

		std::cout << "Chosen Stone Asteroid Value: " << r << std::endl;
	}

}


void StoneAsteroid::Split()
{
	// it only splits if the stone is big or medium otherwise this they dont brake
	// for that you have to check what stone is spawning and compare the values 


	// Check if the world is locked
	if (!World::GetInstance()->GetWorld()->IsLocked()) {
		// Create three new smaller asteroids
		StoneAsteroid* a1 = new StoneAsteroid(new Properties("StoneSmall", (rand() % (840 - 160 + 1) + 160), -100, 96, 96, SDL_FLIP_NONE));
		StoneAsteroid* a2 = new StoneAsteroid(new Properties("StoneSmall", (rand() % (840 - 160 + 1) + 160), -100, 96, 96, SDL_FLIP_NONE));
		StoneAsteroid* a3 = new StoneAsteroid(new Properties("StoneSmall", (rand() % (840 - 160 + 1) + 160), -100, 96, 96, SDL_FLIP_NONE));

		// Set their size to small
		a1->size_ = Small;
		a2->size_ = Small;
		a3->size_ = Small;
		// Set their health to the appropriate value
		a1->m_MaxHealh = 1;
		a2->m_MaxHealh = 1;
		a3->m_MaxHealh = 1;
		// Give each smaller asteroid an initial velocity
		a1->m_Body->SetLinearVelocity(b2Vec2(5, 5));
		a2->m_Body->SetLinearVelocity(b2Vec2(-5, -5));
		a3->m_Body->SetLinearVelocity(b2Vec2(5, -5));
		// Add the smaller asteroids to the game world
		World::GetInstance()->LoadObjects(a1);
		World::GetInstance()->LoadObjects(a2);
		World::GetInstance()->LoadObjects(a3);
		// Remove the current asteroid from the game world
		Clean();

	}
}


void StoneAsteroid::Explosion()
{
	b_IsDead = true;
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
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}

}
