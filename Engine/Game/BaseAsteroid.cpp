#include "BaseAsteroid.h"
#include "World.h"
#include "Animation.h"
#include "Renderer.h"
#include "World.h"

BaseAsteroid::BaseAsteroid(Properties* props) : Character(props)
{
	m_Animation = new Animation();
}

void BaseAsteroid::Init()
{
	__super::Init();
	SetupBody();
}

void BaseAsteroid::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}

void BaseAsteroid::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void BaseAsteroid::Clean()
{
	delete m_Animation;
}

void BaseAsteroid::CheckColision()
{
	// todo in specific asteroid
}

void BaseAsteroid::TakeDamage(int inDamage)
{
/*
// 	if (size_ != Small)
// 	{
// 		health_ -= inDamage;
// 
// 		if (health_ <= 0)
// 		{
// 			Split();
// 		}
// 	}
*/
}

void BaseAsteroid::GiveDamage()
{
}

void BaseAsteroid::SetupBody()
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		b2BodyDef _BodyDef;
		_BodyDef.type = b2_kinematicBody;
		_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
		_BodyDef.gravityScale = 0.0f;
		_BodyDef.fixedRotation = true;
		_BodyDef.bullet = true;


		m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

		b2PolygonShape _boxShape;
		_boxShape.SetAsBox(m_Width / 2, m_Height / 2);

		b2FixtureDef _fixtureDef;
		_fixtureDef.shape = &_boxShape;
		_fixtureDef.density = 1.0f;
		_fixtureDef.isSensor = true;
		_fixtureDef.userData.pointer = (uintptr_t)this;


		b2Fixture* _Fixture;
		_Fixture = m_Body->CreateFixture(&_fixtureDef);
	}
}

void BaseAsteroid::SetAnimationState(BaseAsteroid inCurrentAnimationState, float inAxisValue)
{
}

void BaseAsteroid::SetOriginPoint()
{
}

void BaseAsteroid::Split()
{
	// to eliminate the sprite and sow other in the same position 
}
