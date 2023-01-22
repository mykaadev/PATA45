#include "StoneAsteroid.h"
#include "World.h"
#include "EngineTime.h"
#include "Player.h"
#include <iostream>



Uint32 SplitIntoSmallAsteroids(Uint32 interval, void* data)
{
	StoneAsteroid* asteroid1 = nullptr;
	StoneAsteroid* asteroid2 = nullptr;
	StoneAsteroid* asteroid3 = nullptr;

	asteroid1 = new StoneAsteroid(new Properties("Asteroid1",
		dynamic_cast<GameObject*>((StoneAsteroid*)data)->m_Body->GetPosition().x - 40,
		dynamic_cast<GameObject*>((StoneAsteroid*)data)->m_Body->GetPosition().y - 40, 32, 32));

	asteroid2 = new StoneAsteroid(new Properties("Asteroid2",
		dynamic_cast<GameObject*>((StoneAsteroid*)data)->m_Body->GetPosition().x + 40,
		dynamic_cast<GameObject*>((StoneAsteroid*)data)->m_Body->GetPosition().y - 40, 32, 32));

	asteroid3 = new StoneAsteroid(new Properties("Asteroid3",
		dynamic_cast<GameObject*>((StoneAsteroid*)data)->m_Body->GetPosition().x,
		dynamic_cast<GameObject*>((StoneAsteroid*)data)->m_Body->GetPosition().y - 80, 32, 32));

	asteroid1->m_HasBeenSplit = true;
	asteroid2->m_HasBeenSplit = true;
	asteroid3->m_HasBeenSplit = true;

	World::GetInstance()->LoadObjects(asteroid1);
	World::GetInstance()->LoadObjects(asteroid2);
	World::GetInstance()->LoadObjects(asteroid3);
	
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		asteroid1->GetBody()->SetLinearVelocity(b2Vec2(-1.0f, 1.0f));
		asteroid2->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 1.0f));
		asteroid3->GetBody()->SetLinearVelocity(b2Vec2(1.0f, 1.0f));
	}


	EngineTime::GetInstance()->RemoveTimer(dynamic_cast<StoneAsteroid*>((StoneAsteroid*)data)->myTimerID);

	return interval;
}



StoneAsteroid::StoneAsteroid(Properties* props) : BaseAsteroid(props)
{
	r = 0;
	m_HasBeenSplit = false;
	m_DamageAmount = 2;
}

void StoneAsteroid::Init()
{
	__super::Init();

	if (!m_HasBeenSplit) { ChooseType(); }
	
	if (m_HasBeenSplit)
	{
		m_Animation->SetProperties("StoneSmall", 1, 0, 16, 50, true);
		SetSize(32, 32);
		m_CurrentHealth = 1;
		size_ = Small;
	}
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
		int randX = 0;
		//int randY = rand() % 8;
		int randY = 1;
		int randVelocity = 0;

		if (randX == 0 && !World::GetInstance()->GetWorld()->IsLocked()) {
			m_Body->SetLinearVelocity(b2Vec2(-randVelocity, randY));
		}
		else if (!World::GetInstance()->GetWorld()->IsLocked()) {
			m_Body->SetLinearVelocity(b2Vec2(randVelocity, randY));
		}
		
		//Handle Out of screen destroy
		if (m_Body->GetPosition().y > 700.0f && !m_IsDead && m_Body != nullptr)
		{
			m_IsDead = true;
			if (!World::GetInstance()->GetWorld()->IsLocked())
			{
				m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			}
			
			m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 100, false);
			SetSize(64, 64);
			if (m_Animation->GetAnimationFinished())
			{
				Clean();
			}
			 
		}
	}


	m_Animation->Update(deltaTime);
}



void StoneAsteroid::TakeDamage(int damage)
{

	m_CurrentHealth -= damage;
	
  	if (m_CurrentHealth <= 0 && !m_IsDead)
	{
		Explosion();	
	}
}

/* Goes to the player and gives damage  */
void StoneAsteroid::GiveDamage()
{
	
}

/* follows the same logic of the metal Asteroid */
void StoneAsteroid::ChooseType()
{

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		int r = rand() % 3;

		switch (r) {
		case 0:
			size_ = Big;
			m_Animation->SetProperties("Stone", 1, 0, 25, 50, true);
			SetSize(96, 96);
			m_CurrentHealth = 3;
			break;
		case 1:
			size_ = Medium;
			m_Animation->SetProperties("StoneMedium", 1, 0, 24, 50, true);
			SetSize(64, 64);
			m_CurrentHealth = 2;
			break;
		case 2:
			size_ = Small;
			m_Animation->SetProperties("StoneSmall", 1, 0, 16, 50, true);
			SetSize(32, 32);
			m_CurrentHealth = 1;
			break;

		default:
			size_ = Small;
			m_Animation->SetProperties("StoneSmall", 1, 0, 16, 200, true);
			SetSize(32, 32);
			m_CurrentHealth = 1;
			break;
		}
	}
}


void StoneAsteroid::Explosion()
{
	m_IsDead = true;

	if (size_ == Big || size_ == Medium)
	{
		myTimerID = EngineTime::GetInstance()->StartTimer(100, SplitIntoSmallAsteroids, (StoneAsteroid*)this);
	}

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
}

void StoneAsteroid::Clean()
{
	m_PendingKill = true;
}

void StoneAsteroid::CheckCollision(GameObject* otherGameObject)
{
	if (dynamic_cast<Player*>(otherGameObject) && !m_PendingKill && !dynamic_cast<Player*>(otherGameObject)->GetIsDead() && !collided)
	{
		((Player*)otherGameObject)->TakeDamage(m_DamageAmount);
		collided = true;

		Explosion();
	}
}

/* Set the position */
void StoneAsteroid::SetOriginPoint()
{
	if (m_Body != nullptr)
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
}

