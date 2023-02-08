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
	float color = fabsf(sinf(count * 0.05f)) * 0.7f + 0.2f;
	if (isRight)
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.8f,0.8f,0.1f,color });
	}
	else
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.8f,0.8f,0.1f,color-0.1f });
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
	if(texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowYellow.png", texhandle[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowRed.png", texhandle[1]);
	}
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

void PredictBlockManager::DrawArrowLeft(Camera* camera, Vec3 pos, float scale, XMFLOAT4 color, bool reverseX)
{
	arrowObjLeft.DrawBoxSprite(pos, scale, color, texhandle[1], Vec2(0.0f, 0.0f), true,false,0.0f);
}

void PredictBlockManager::DrawArrowRight(Camera* camera,Vec3 pos, float scale, XMFLOAT4 color,bool reverseX)
{
	arrowObjRight.DrawBoxSprite(pos, scale, color, texhandle[0], Vec2(0.0f, 0.0f), false, false, 0.0f);
	
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


