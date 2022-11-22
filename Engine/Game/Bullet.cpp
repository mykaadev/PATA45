#include "Bullet.h"
#include "World.h"

Bullet::Bullet(Properties* props) : Character(props) {

	m_Animation = new Animation();
}

void Bullet::Init()
{
	SetupBody();
	SetAnimationState(travelling, 0);
}

void Bullet::Draw()
{
	m_Animation->Draw(m_BulletBody->GetPosition().x, m_BulletBody->GetPosition().y, m_Width, m_Height);
}

void Bullet::Update(float deltaTime)
{
	SetOriginPoint();
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

	m_BulletBody = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(1.0f, 1.0f);

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.3f;
	b2Fixture* _Fixture;
	_Fixture = m_BulletBody->CreateFixture(&_fixtureDef);


}

void Bullet::SetAnimationState(BulletStates inCurrentAnimationState, float inAxisValue)
{
	m_Animation->SetProperties("BulletOne", 1, 0, 2, 100, true);
}

void Bullet::HandleMove(const int x_border, const int y_border)
{
	// movement and speddddddx
}

// position
void Bullet::SetOriginPoint()
{
	if (m_BulletBody == nullptr) return;

	m_Origin->X = m_BulletBody->GetPosition().x + m_Width / 2;
	m_Origin->Y = m_BulletBody->GetPosition().y - m_Height / 2;
}
Bullet::~Bullet()
{

}

