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


	//Œq‚®ˆ—
	if (isConnect_ == false && isOverlap_ == false && isRotate_ == false)
	{
		isConnect_ = true;
	}
	//‰ñ“]‚·‚éˆ—
	if (isConnect_ == false && isOverlap_ == false)
	{

	}

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
