#include "PredictBlock.h"


PredictBlock::~PredictBlock()
{

}

PredictBlock& PredictBlock::operator=(const PredictBlock& obj)
{
	this->obj = obj.obj;
	count = obj.count;
	model = obj.model;
	worldTransform_ = obj.worldTransform_;
	isRight = obj.isRight;

	return *this;
}

void PredictBlock::Initialize(Vec3 pos, Vec3 scale, bool isRight, Model* model)
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
	float color = fabsf(sinf(count * 0.02f)) * 0.9f + 0.0f;
	if (isRight)
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.80f,0.70f,0.1f,color });
	}
	else
	{
		obj.DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 0.80f,0.70f,0.1f,color - 0.1f });
	}
}

//-------------------------------------------------------------------------------

PredictBlockManager::~PredictBlockManager()
{
	predictArrows_.clear();
	predictBlocks_.clear();
}

PredictBlockManager& PredictBlockManager::operator=(const PredictBlockManager& obj)
{
	count = obj.count;
	//中身のみコピー
	this->predictBlocks_.clear();
	for (std::list<std::unique_ptr<PredictBlock>>::const_iterator itr = obj.predictBlocks_.begin(); itr != obj.predictBlocks_.end(); itr++)
	{
		std::unique_ptr<PredictBlock> p = std::make_unique<PredictBlock>();

		p->operator=(*itr->get());

		predictBlocks_.push_back(std::move(p));

		p.reset();
	}

	this->predictArrows_.clear();
	for (std::list<std::unique_ptr<PredictArrow>>::const_iterator itr = obj.predictArrows_.begin(); itr != obj.predictArrows_.end(); itr++)
	{
		std::unique_ptr<PredictArrow> p = std::make_unique<PredictArrow>();

		p->operator=(*itr->get());

		predictArrows_.push_back(std::move(p));

		p.reset();
	}

	return *this;
}

void PredictBlockManager::Initialize()
{
	//矢印用の画像読み込み
	if (texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowRight.png", texhandle[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowLeft.png", texhandle[1]);
	}

	count = 0;
	ClearPredictBlock();
}

void PredictBlockManager::Update()
{
	count++;


	for (std::unique_ptr<PredictBlock>& pB : predictBlocks_)
	{
		pB->Update(this->count);
	}

	for (std::unique_ptr<PredictArrow>& pA : predictArrows_)
	{
		pA->Update(this->count);
	}
}


void PredictBlockManager::Draw(Camera* camera, bool isArrowDraw)
{
	for (std::unique_ptr<PredictBlock>& pB : predictBlocks_)
	{
		pB->Draw(camera);
	}

	for (std::unique_ptr<PredictArrow>& pA : predictArrows_)
	{
		pA->Draw(camera, isArrowDraw, this->texhandle);
	}
}

void PredictBlockManager::AddPredictBlock(Vec3 pos, Vec3 scale, bool isRight, Model* model)
{
	std::unique_ptr<PredictBlock> predictBlock = std::make_unique<PredictBlock>();
	predictBlock->Initialize(pos, scale, isRight, model);
	this->predictBlocks_.push_back(std::move(predictBlock));
}

void PredictBlockManager::AddPredictArrow(Vec3 pos, Vec3 scale)
{
	std::unique_ptr<PredictArrow> predictArrow = std::make_unique<PredictArrow>();
	predictArrow->Initialize(pos, scale);
	this->predictArrows_.push_back(std::move(predictArrow));
}

void PredictBlockManager::ClearPredictBlock()
{
	this->predictBlocks_.clear();

	this->predictArrows_.clear();

}


//------------------------------------------------------------------
PredictArrow& PredictArrow::operator=(const PredictArrow& obj)
{
	count = obj.count;
	for (int i = 0; i < _countof(obj.obj); i++)
	{
		this->obj[i] = obj.obj[i];
	}

	worldTransform_ = obj.worldTransform_;

	scaleArrow = obj.scaleArrow;
	colorAlpha = obj.colorAlpha;

	return *this;
}

void PredictArrow::Initialize(Vec3 pos, Vec3 scale)
{
	worldTransform_.trans = pos;
	worldTransform_.scale = scale;
	worldTransform_.SetWorld();
}

void PredictArrow::Update(int count)
{
	if (colorAlpha < 0.65f)
	{
		colorAlpha += 0.01f;
	}
	else
	{
		colorAlpha = 0.65f;
	}
	this->count = count;
}

void PredictArrow::Draw(Camera* camera, bool isArrowDraw, UINT64* texhandle)
{
	Vec2 posNum = Vec3toVec2(worldTransform_.trans, camera->viewMat.matView, camera->projectionMat.matProjection);
	Vec3 pos = Vec3(posNum.x + 180, posNum.y + 225, 0.0f);

	float scale = fabsf(sinf(count * 0.011f)) * 0.011f + 0.22f;

	if (isArrowDraw)
	{
		//右
		obj[0].DrawBoxSprite(pos, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, colorAlpha), texhandle[0], Vec2(0.5f, 0.5f), false, false, 0.0f);
		//左
		pos.x -= 180 * 2;
		obj[1].DrawBoxSprite(pos, scale, XMFLOAT4(0.5f, 0.5f, 0.5f, colorAlpha), texhandle[1], Vec2(0.5f, 0.5f), false, false, 0.0f);
	}
}

void PredictArrow::fadeColor()
{
	colorAlpha -= 0.01f;
}
