#include "Block.h"

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
	worldTransform_.SetWorld();
}

void Block::Draw(Camera* camera)
{
}
