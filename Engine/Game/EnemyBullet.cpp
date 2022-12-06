#include "EnemyBullet.h"
#include "Player.h"


EnemyBullet::EnemyBullet(Properties* props) : Character(props) {

	m_Animation = new Animation();
	m_damageAmount = { 0 };  // TESTING PURPOSES

}

void EnemyBullet::Init()
{
	SetupBody();
	m_Animation->SetProperties("EnemyBullet", 1, 0, 2, 100, true);
	m_Body->SetLinearVelocity(b2Vec2(0.0f, 2.0f));
	//FIND AN LONER AND GET HIS POSITION SO WE CAN SET IT
}



void EnemyBullet::SetupBody()
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


	b2Fixture* _Fixture;

	_Fixture = m_Body->CreateFixture(&_fixtureDef);
}



void EnemyBullet::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animation->Update(deltaTime);

	SetOriginPoint();

	if (m_Body->GetPosition().y >= 640.0f)
	{
		m_IsDead = true;
	}

	if (m_IsDead)
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		m_Animation->SetProperties("Explosion", 1, 0, 11, 50, false);

		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}
}


void EnemyBullet::Draw()
{
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
	//World::GetInstance()->DestroyGameObject(this, m_Body);
}

void EnemyBullet::CheckCollision(GameObject* otherGameObject)
{

	if (dynamic_cast<Player*>(otherGameObject))
	{
		((Player*)otherGameObject)->TakeDamage(m_damageAmount);
	}
	else
	{
		return;
	}
	m_IsDead = true;
}


EnemyBullet::~EnemyBullet()
{
	delete m_Animation;
}

