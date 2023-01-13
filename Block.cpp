#include "Block.h"

const int blockWidth = 13;
const int blockHeight = 13;

void Block::Initialize(Model* model, DebugText* debugText_)
{
	assert(model);

	model_ = model;
	this->debugText_ = debugText_;
	
	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.SetWorld();

	radius_ = scaleTmp;
}

void Block::Updata()
{
	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.SetWorld();
}

void Block::Draw(Camera* camera)
{
}

void Block::OnCollision(Collider& collider)
{
}

void Block::OnCollision2(Collider& collider)
{
}
