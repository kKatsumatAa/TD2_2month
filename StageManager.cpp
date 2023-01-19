#include "StageManager.h"

using namespace std;

void StageManager::Initialize(BlockManager* blockManager)
{
	//インスタンスの生成
	//blockManager_ = std::make_unique<BlockManager>();
	blockManager_ = blockManager;

	int tutorialWidth = 13;
	int tutorialHeight = 13;

	//ベクタ配列に要素<ワールド行列>を追加
	for (int i = 0; i < tutorialWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		worldmats_.push_back(vector<WorldMat>());

		for (int j = 0; j < tutorialHeight; j++)
		{
			//ブロックの要素を追加
			worldmats_[i].push_back(worldmat_);
		}
	}
	
	//ベクタ配列に要素<形>を追加
	for (int i = 0; i < tutorialWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		forms_.push_back(vector<Block::Form>());

		for (int j = 0; j < tutorialHeight; j++)
		{
			//形の要素を追加
			forms_[i].push_back(form_);
		}
	}
}

void StageManager::Update()
{

}

void StageManager::SetWorldMat(const int& blockWidth, const int& blockHeight)
{
	//ブロックの半径
	float blockRadius_ = blockManager_->GetRadius();

	//縦幅、横幅
	int tutorialWidth = blockWidth;
	int tutorialHeight = blockHeight;

	//横幅、縦幅の要素数を変更

	//縦幅
	worldmats_.resize(tutorialWidth);
	//横幅
	for (int i = 0; i < tutorialWidth; i++)
	{
		worldmats_[i].resize(tutorialHeight);
	}
	//縦幅
	forms_.resize(tutorialWidth);
	//横幅
	for (int i = 0; i < tutorialWidth; i++)
	{
		forms_[i].resize(tutorialHeight);
	}

	//ワールド行列の設定
	for (int i = 0; i < tutorialWidth; i++)
	{
		for (int j = 0; j < tutorialHeight; j++)
		{
			worldmats_[i][j].scale = { blockRadius_ ,blockRadius_,blockRadius_ };

			//ブロックの座標を設定
			if (i >= 0)
			{
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);
			}
			if (j >= 0)
			{
				worldmats_[i][j].trans.z = j * (worldmats_[i][j].scale.y * 2.0f);
			}

			worldmats_[i][j].SetWorld();
		}
	}

}

void StageManager::SetTutorial(const int& blockWidth, const int& blockHeight)
{
	SetWorldMat(blockWidth, blockHeight);

	//形の設定
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BUTTON,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BUTTON,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BUTTON,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};
	
	//行列をセット
	blockManager_->SetStage(blockWidth, blockHeight,worldmats_,forms_);
}

void StageManager::SetStage1(const int& blockWidth, const int& blockHeight)
{
	SetWorldMat(blockWidth, blockHeight);

	//形の設定
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BUTTON,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BUTTON,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BUTTON,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};

	//行列をセット
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}
