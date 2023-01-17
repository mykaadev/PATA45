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


StoneAsteroid::StoneAsteroid(Properties* props, Size size) : BaseAsteroid(props)
{

	m_GoingRight = true;
	m_Body->SetLinearVelocity(b2Vec2(0.f, 1.5f));

	switch (size)
	{
	case BaseAsteroid::Big:
		m_Animation->SetProperties("Stone", 1, 0, 25, 50, true);

		break;
	case BaseAsteroid::Medium:
		m_Animation->SetProperties("StonelMedium", 1, 0, 24, 50, true);

		break;
	case BaseAsteroid::Small:
		m_Animation->SetProperties("StoneSmall", 1, 0, 16, 50, true);
		break;
	default:
		break;
	}
}

void StoneAsteroid::Init()
{
	__super::Init();
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
	if (size_ != Small)
	{
	}
}

void StoneAsteroid::GiveDamage()
{
}

void StoneAsteroid::SpawnAsteroid()
{
	int r = rand() % 3;
	switch (r) {
	case 0:
		size_ = Big;
		m_Animation->SetProperties("Stone", 1, 0, 25, 50, true);
		break;
	case 1:
		size_ = Medium;
		m_Animation->SetProperties("StonelMedium", 1, 0, 24, 50, true);
		break;
	case 2:
		size_ = Small;
		m_Animation->SetProperties("StoneSmall", 1, 0, 16, 50, true);
		break;

	}

}

void StoneAsteroid::Split()
{

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
