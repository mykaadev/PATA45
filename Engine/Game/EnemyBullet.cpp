#include "EnemyBullet.h"
#include "Player.h"


EnemyBullet::EnemyBullet(Properties* props) : Character(props) {

	m_Animation = new Animation();
	m_damageAmount = { 1 };
}

void EnemyBullet::Init()
{
	SetupBody();
	m_Animation->SetProperties("EnemyBullet", 1, 0, 2, 100, true);

	if (m_Body != nullptr)
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 3.0f));
	}
}



void EnemyBullet::SetupBody()
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{

		b2BodyDef _BodyDef;
		_BodyDef.type = b2_dynamicBody;
		_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
		_BodyDef.gravityScale = 0.0f;
		_BodyDef.fixedRotation = true;
		_BodyDef.bullet = true;

		m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

		b2PolygonShape _boxShape;
		_boxShape.SetAsBox(m_Width / 2 - 4, m_Height / 2 - 4);

		b2FixtureDef _fixtureDef;
		_fixtureDef.shape = &_boxShape;
		_fixtureDef.isSensor = true;
		_fixtureDef.userData.pointer = (uintptr_t)this;



		if (!World::GetInstance()->GetWorld()->IsLocked())
		{
			b2Fixture* _Fixture;

			_Fixture = m_Body->CreateFixture(&_fixtureDef);
		}

	}
}



void EnemyBullet::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animation->Update(deltaTime);

	SetOriginPoint();

	if (m_Body == nullptr) return;

	if (m_Body->GetPosition().y >= 640.0f && !m_IsDead) 
	{
		m_IsDead = true;
		m_Animation->SetProperties("Explosion", 1, 0, 11, 50, false);
		m_Animation->SetCurrentSprite(0);
	}

	if (m_IsDead)
	{
		if (m_Body != nullptr)
		{
			m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}

		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}
}


void EnemyBullet::Draw()
{
	if (m_Body == nullptr) { return; }
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void EnemyBullet::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}


void EnemyBullet::Clean()
{
	m_PendingKill = true;
}

void EnemyBullet::CheckCollision(GameObject* otherGameObject)
{

	if (dynamic_cast<Player*>(otherGameObject) && !collided)
	{
		((Player*)otherGameObject)->TakeDamage(m_damageAmount);
		collided = true;
		m_IsDead = true;
		m_Animation->SetProperties("Explosion", 1, 0, 11, 50, false);
		m_Animation->SetCurrentSprite(0);
	}
	
	if (dynamic_cast<Companion*>(otherGameObject) && !m_PendingKill && !dynamic_cast<Companion*>(otherGameObject)->GetIsDead() && !collided)
	{
		((Companion*)otherGameObject)->TakeDamage(1);
		collided = true;
		m_IsDead = true;
		m_Animation->SetProperties("Explosion", 1, 0, 11, 50, false);
		m_Animation->SetCurrentSprite(0);
	}
	
}


EnemyBullet::~EnemyBullet()
{
	delete m_Animation;
}

