#include "PredictBlock.h"


void PredictBlock::Initialize(Vec3 pos, Vec3 scale)
{
	worldTransform_.trans = pos;
	worldTransform_.scale = scale;
	worldTransform_.SetWorld();

}

void PredictBlock::Update(int count)
{
}

void PredictBlock::Draw(Camera* camera)
{
}

//-------------------------------------------------------------------------------
void PredictBlockManager::Initialize()
{
}

void PredictBlockManager::Update()
{
}

void PredictBlockManager::Draw(Camera* camera)
{
}

void PredictBlockManager::AddPredictBlock(Vec3 pos, Vec3 scale)
{
}

void PredictBlockManager::ClearPredictBlock()
{
}


