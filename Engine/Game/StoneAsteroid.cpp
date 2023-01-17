#include "StoneAsteroid.h"
#include "World.h"
#include "EngineTime.h"
#include <iostream>

Uint32 StoneSpawn(Uint32 interval, void* data) {

// 	if (!World::GetInstance()->GetWorld()->IsLocked()) {
// 
// 		EnemyBullet* bullet = nullptr;
// 
// 		bullet = new EnemyBullet(new Properties("EnemyBullet",
// 			dynamic_cast<GameObject*>((LonerEnemy*)data)->m_Body->GetPosition().x,
// 			dynamic_cast<GameObject*>((LonerEnemy*)data)->m_Body->GetPosition().y + 50, 16, 16, SDL_FLIP_NONE));
// 
// 		World::GetInstance()->LoadObjects(bullet);
// 	}

	return interval;
}


StoneAsteroid::StoneAsteroid(Properties* props) : BaseAsteroid(props)
{

	m_GoingRight = true;
	m_Body->SetLinearVelocity(b2Vec2(0.f, 1.5f));
	r = 0;
	m_CurrentHealth = m_MaxHealh;
}

void StoneAsteroid::Init()
{
	__super::Init();
	SetupBody();
	m_Animation->SetProperties("StoneSmall", 1, 0, 16, 100, true);

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
}



void StoneAsteroid::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
	if (size_ != Small)
	{
		//TODO	finishing this part and make the split , check collision and all that
		// if u got time in the morning see how to make the player life and all that 
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
}



void StoneAsteroid::Clean()
{
	EngineTime::GetInstance()->RemoveTimer(myTimerID);
	m_PendingKill = true;
}

void StoneAsteroid::CheckCollision(GameObject* otherGameObject)
{
}


void StoneAsteroid::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}
