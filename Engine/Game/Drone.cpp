
#include "Drone.h"
#include "World.h"
#include "EngineTime.h"
#include <iostream>

Drone::Drone(Properties* props) : BaseEnemy(props)
{
	m_SinCounter = 5.0f;
	m_Speed = 1.5f;
	y = 0;
	m_CurrentHealth = 1;
}

void Drone::Init()
{
	__super::Init();

	// RUBEN DEPOIS TIRA ISTO QUANDO TIVERES A FAZER O MOVIMENTO
	//m_Body->SetLinearVelocity(b2Vec2(1.0f, 0.1f));
	
	m_Animation->SetProperties("Drone", 1, 0, 16,100, true);

}

void Drone::Draw()
{
	__super::Draw();
}
//////////////////////////////RUBEN USA ISTO PARA DARES SET NA POSIÇÃO DELE
// // RUBEN DEPOIS NAO TE ESQUEÇAS DE TIRAR O SET LINEAR NAO SEI QUE DO INIT() QUANDO TIVERES A TESTAR O MOVIMENTO
//m_Body->SetTransform(b2Vec2( AQUI VAI O X, AQUI VAI O Y), m_Body->GetAngle());
void Drone::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();


	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		if (!m_IsDead && m_Body != nullptr)
		{


			float x = m_Body->GetPosition().x;
			float y = m_Body->GetPosition().y;
			m_amplitude = 5.0f; // adjust this value to change the amplitude of the sine wave
			float frequency = 5.0f; // adjust this value to change the frequency of the sine wave
			//float offset = EngineTime::GetInstance()->GetDeltaTime() * frequency;
			

			// set the body's linear velocity
			//m_Body->SetLinearVelocity(b2Vec2(offset,amplitude ));

			// calculate vertical sinusoidal movement
			float offset = EngineTime::GetInstance()->GetDeltaTime() * frequency;
			float verticalVelocity = amplitude * sin(offset);
			float horizontalVelocity = amplitude * sin(offset);

			//m_Body->SetLinearVelocity(b2Vec2(horizontalVelocity, verticalVelocity));
			//m_Body->SetLinearVelocity(b2Vec2((x + m_amplitude * sinf(EngineTime::GetInstance()->GetDeltaTime() * frequency), (y + m_amplitude * sinf(EngineTime::GetInstance()->GetDeltaTime() * frequency))), 0));


			/*
	
			//m_Body->SetTransform(b2Vec2(x + m_amplitude * sinf(EngineTime::GetInstance()->GetDeltaTime() * frequency), y), 0);
			m_Body->SetLinearVelocity(b2Vec2((x + m_amplitude * sinf(EngineTime::GetInstance()->GetDeltaTime() * frequency), y), 0));

			//float offset = EngineTime::GetInstance()->GetDeltaTime() * frequency;
			//float horizontalVelocity = amplitude * sin(offset);


			//m_Body->SetTransform(b2Vec2(horizontalVelocity,m_Body->GetPosition().y ), m_Body->GetAngle());
			//m_Body->SetTransform(b2Vec2(horizontalVelocity, m_Body->GetPosition().y), m_Body->GetAngle());

			//m_Body->SetLinearVelocity(b2Vec2(horizontalVelocity, m_Body->GetLinearVelocity().y));
			 */



			// handle out of screen destroy
			if (m_Body->GetPosition().x < -500.0f - m_Width / 2 || m_Body->GetPosition().x > 960.0f + m_Width / 2)
			{
				m_IsDead = true;
			
				m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				
				m_Animation->SetCurrentSprite(0);
				m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 50, false);
				SetSize(64, 64);
			}
		}

		if (m_IsDead)
		{
			if (GetAnimation()->GetCurrentSprite() >= 10)
			{
				Clean();
			}
		}
	}


	

}

void Drone::Clean()
{
	m_PendingKill = true;
}

	


void Drone::CheckCollision(GameObject* otherGameObject)
{

}

void Drone::TakeDamage(int damage)
{
	
	m_CurrentHealth -= damage;
	
	if (m_CurrentHealth <= 0)
	{

		Explosion();
	}
}

void Drone::SetOriginPoint()
{


	if (m_Body == nullptr) return;
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}

}


void Drone::Explosion()
{
	m_IsDead = true;
	
	m_Animation->SetCurrentSprite(0);
	m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
	SetSize(64, 64);
}
