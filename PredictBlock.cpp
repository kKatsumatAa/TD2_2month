#include "PredictBlock.h"


void PredictBlock::Initialize(Vec3 pos, Vec3 scale,bool isRight, Model* model)
{
	worldTransform_.trans = pos;
	worldTransform_.scale = scale;
	worldTransform_.SetWorld();

	this->isRight = isRight;

	this->model = model;
}

void PredictBlock::Update(int count)
{
	this->count = count;
}

void PredictBlock::Draw(Camera* camera)
{
	float color = fabsf(sinf(count * 0.05f)) * 0.7f;
	if (isRight)
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.6f,0.1f,0.1f,color });
	}
	else
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.1f,0.1f,0.6f,color });
	}
}

//-------------------------------------------------------------------------------
//PredictBlockManager& PredictBlockManager::operator=( PredictBlockManager& obj)
//{
//	predictBlocks_.clear();
//
//	this->count = obj.count;
//	for (PredictBlock& pB : obj.predictBlocks_)
//	{
//		this->predictBlocks_.push_back(pB);
//	}
//
//	return *this;
//}

void PredictBlockManager::Initialize()
{
	count = 0;
	ClearPredictBlock();
}

void PredictBlockManager::Update()
{
	count++;

	for (PredictBlock& pB : predictBlocks_)
	{
		pB.Update(this->count);
	}
}

void PredictBlockManager::Draw(Camera* camera)
{
	for (PredictBlock& pB : predictBlocks_)
	{
		pB.Draw(camera);
	}
}

void PredictBlockManager::AddPredictBlock(Vec3 pos, Vec3 scale, bool isRight, Model* model)
{
	PredictBlock predictBlock;
	predictBlock.Initialize(pos, scale,isRight, model);
	this->predictBlocks_.push_back(predictBlock);
}

void PredictBlockManager::ClearPredictBlock()
{
	this->predictBlocks_.clear();
}


