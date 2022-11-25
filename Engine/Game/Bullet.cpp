#include "Bullet.h"
#include "World.h"
#include "TextureManager.h"

Bullet::Bullet(Properties* props) : Character(props) {

	m_Animation = new Animation();
}

void Bullet::Init()
{
	SetupBody();
	SetAnimationState(travelling, 0);
	m_Body->ApplyLinearImpulse(b2Vec2(0.0f, -1000.0f), m_Body->GetPosition(), true);
}

void Bullet::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}

void Bullet::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();
	if (m_Body->GetPosition().y < 50.0f )
	{
		Clean();
	}

	m_Animation->Update(deltaTime);
}

void Bullet::Clean()
{
	
}



void Bullet::SetupBody()
{
	b2BodyDef _BodyDef;
	_BodyDef.type = b2_dynamicBody;
	_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
	_BodyDef.gravityScale = 0.0f;

	//is bullet
	_BodyDef.bullet= true;


	m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(1.0f, 1.0f);

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.3f;
	b2Fixture* _Fixture;
	_Fixture = m_Body->CreateFixture(&_fixtureDef);


}


void Bullet::CheckColision()
{
	//make the check for collsion
	m_Body->SetBullet(true);

}

void Bullet::SetAnimationState(BulletStates inCurrentAnimationState, float inAxisValue)
{
	m_Animation->SetProperties("BulletOne", 1, 0, 2, 100, true);
}



// position
void Bullet::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	m_Origin->X = m_Body->GetPosition().x + m_Width / 2;
	m_Origin->Y = m_Body->GetPosition().y - m_Height / 2;
}

Bullet::~Bullet()
{
	delete m_Animation;
	World::GetInstance()->Destroy(m_Body);
}

