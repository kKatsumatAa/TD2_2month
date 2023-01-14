#include "Block.h"

const int blockWidth = 13;
const int blockHeight = 13;



Block::~Block()
{
	
}

void Block::Initialize()
{
	//assert(model);
	//model_ = model;
	//this->debugText_ = debugText_;
	
	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.SetWorld();

	radius_ = scaleTmp;
}

void Block::Updata()
{	
	//現在使ってない
	//worldTransform_.SetWorld();
}

void Block::Draw(Camera* camera)
{
	//仮表示
	draw->DrawCube3D(&worldTransform_, &camera->viewMat, &camera->projectionMat);
	//model_[0] = Model::LoadFromOBJ("sphere");
	//draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model_[0]);
}

void Block::OnCollision(Collider& collider)
{
}

void Block::OnCollision2(Collider& collider)
{
}

void Block::SetWorldPos(Vec3& pos)
{
	worldTransform_.trans = pos;
	worldTransform_.SetWorld();
}
