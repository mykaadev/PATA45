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
	if (m_Body != nullptr)
	{
		m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
	}
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
	// todo in specific asteroid type
}

void BaseAsteroid::TakeDamage(int inDamage)
{
	// todo in the specific asteroid type
}

void BaseAsteroid::GiveDamage()
{
	// todo in the specific asteroid type
}

void BaseAsteroid::ChooseType(){}

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

void BaseAsteroid::SetAnimationState(AsteroidSpinning inCurrentAnimationState, float inAxisValue)
{
	//Set the animation for them 
}

void BaseAsteroid::SetOriginPoint()
{
	// set the origin point 
}

