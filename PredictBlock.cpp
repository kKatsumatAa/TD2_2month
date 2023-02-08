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
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.80f,0.70f,0.1f,color });
	}
	else
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.80f,0.70f,0.1f,color-0.1f });
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

	for (PredictArrow& pA : predictArrows_)
	{
		pA.Draw(camera);
	}
}

void PredictBlockManager::AddPredictBlock(Vec3 pos, Vec3 scale, bool isRight, Model* model)
{
	PredictBlock predictBlock;
	predictBlock.Initialize(pos, scale,isRight, model);
	this->predictBlocks_.push_back(predictBlock);
}

void PredictBlockManager::AddPredictArrow(Vec3 pos, Vec3 scale)
{
	PredictArrow predictArrow;
	predictArrow.Initialize(pos, scale);
	this->predictArrows_.push_back(predictArrow);
}

void PredictBlockManager::ClearPredictBlock()
{
	this->predictBlocks_.clear();
	this->predictArrows_.clear();
}

void PredictArrow::Initialize(Vec3 pos, Vec3 scale)
{
	worldTransform_.trans = pos;
	worldTransform_.scale = scale;
	worldTransform_.SetWorld();


	if(texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowYellow.png", texhandle[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowRed.png", texhandle[1]);
	}
}

void PredictArrow::Update(int count)
{
}

void PredictArrow::Draw(Camera* camera)
{
	Vec2 posNum = Vec3toVec2(worldTransform_.trans, camera->viewMat.matView, camera->projectionMat.matProjection);
	Vec3 pos = Vec3(posNum.x + 80, posNum.y + 64, 0.0f);

	float scale = 0.3f;
	obj[0].DrawBoxSprite(pos, scale, XMFLOAT4(1.0f,1.0f,1.0f,1.0f), texhandle[0], Vec2(0.5f, 0.5f), false, false, 0.0f);
	pos.x -= 80 * 2;
	obj[1].DrawBoxSprite(pos, scale, XMFLOAT4(1.0f,1.0f,1.0f,1.0f), texhandle[1], Vec2(0.5f, 0.5f), false, false, 0.0f);
}
