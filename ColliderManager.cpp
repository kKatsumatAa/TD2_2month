#include "ColliderManager.h"
#include "Util.h"


void ColliderManager::Initialize()
{
	colliders_.clear();

	this->audio = &Sound::GetInstance();
}

void ColliderManager::Update()
{
}

//---------------------------------------------------------------------------------------------
void ColliderManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetIsDead() || colliderB->GetIsDead())
		|| (colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask())
		|| (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
	{
		return;//����A�Փˏ�������������
	}


	Vec3 posA = colliderA->GetWorldPos();
	Vec3 posB = colliderB->GetWorldPos();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{

	}
}

void ColliderManager::CheckAllCollisions()
{
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrB��itrA�̎��̗v�f����񂷁i�d�����������j
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}

void ColliderManager::SetListCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void ColliderManager::ClearList()
{
	colliders_.clear();
}
