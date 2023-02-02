#include "StageManager.h"

using namespace std;

int StageManager::selectStage = STAGE::TUTORIAL;

StageManager::~StageManager()
{
	delete conectLimit_;
}

void StageManager::Initialize(BlockManager* blockManager
	, Tutorial* tutorial)
{
	//インスタンスの生成
	//blockManager_ = std::make_unique<BlockManager>();
	blockManager_ = blockManager;
	//conectLimit_ = std::make_unique<ConectLimit>();

	conectLimit_ = new ConectLimit;

	if(isCountReset[STAGE::TUTORIAL] == true)
	{
		conectLimit_->SetCount(1);
		conectLimit_->ResetCount();
	}
	else if(isCountReset[STAGE::STAGE1] == true)
	{
		conectLimit_->SetCount(2);
		conectLimit_->ResetCount();
	}
	else if(isCountReset[STAGE::STAGE2] == true)
	{
		conectLimit_->SetCount(3);
		conectLimit_->ResetCount();
	}
	else if(isCountReset[STAGE::STAGE3] == true)
	{
		conectLimit_->SetCount(4);
		conectLimit_->ResetCount();
	}
	else
	{
		conectLimit_->SetCount(0);
		conectLimit_->ResetCount();
	}


	
	this->tutorial = tutorial;

	int tutorialWidth = 13;
	int tutorialHeight = 13;

	//ベクタ配列に要素<ワールド行列>を追加
	for(int i = 0; i < tutorialWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		worldmats_.push_back(vector<WorldMat>());

		for(int j = 0; j < tutorialHeight; j++)
		{
			//ブロックの要素を追加
			worldmats_[i].push_back(worldmat_);
		}
	}

	//ベクタ配列に要素<形>を追加
	for(int i = 0; i < tutorialWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		forms_.push_back(vector<Block::Form>());

		for(int j = 0; j < tutorialHeight; j++)
		{
			//形の要素を追加
			forms_[i].push_back(form_);

			//プレイヤーの位置をリセット
			playerPos[i][j] = false;
		}
	}

	////ここで最初のステージを設定
	//SetTutorial(13,13);
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
	for(int i = 0; i < tutorialWidth; i++)
	{
		worldmats_[i].resize(tutorialHeight);
	}
	//縦幅
	forms_.resize(tutorialWidth);
	//横幅
	for(int i = 0; i < tutorialWidth; i++)
	{
		forms_[i].resize(tutorialHeight);
	}

	//ワールド行列の設定
	for(int i = 0; i < tutorialWidth; i++)
	{
		for(int j = 0; j < tutorialHeight; j++)
		{
			worldmats_[i][j].scale = { blockRadius_ ,blockRadius_,blockRadius_ };

			//ブロックの座標を設定
			if(i >= 0)
			{
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);
			}
			if(j >= 0)
			{
				worldmats_[i][j].trans.z = j * (worldmats_[i][j].scale.y * 2.0f);
			}

			//worldmats_[i][j].trans.y = worldmats_[i][j].scale.y;

			worldmats_[i][j].SetWorld();
		}
	}

}

void StageManager::SetTutorial(const int& blockWidth, const int& blockHeight)
{
	this->tutorial->Initialize();
	stageWidth = blockWidth;
	selectStage = STAGE::TUTORIAL;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//ここでプレイヤーの位置指定
	playerPos[0][0] = true;

	SetisCountReset(selectStage);

	//形の設定
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GEAR,Form::BLOCK,Form::BLOCK,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::GOAL,Form::Electric,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::GEAR,Form::BLOCK,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};

	//行列をセット
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);
}

void StageManager::SetStage1(const int& blockWidth, const int& blockHeight)
{
	this->tutorial->ButtonInitialize();
	stageWidth = blockWidth;
	selectStage = STAGE::STAGE1;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//ここでプレイヤーの位置指定
	playerPos[0][0] = true;

	SetisCountReset(selectStage);

	//形の設定
	forms_ =
	{
		{Form::GEAR,Form::BLOCK,Form::GEAR,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::GEAR,Form::BUTTON,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::Electric,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
				{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},

	};

	//行列をセット
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}

void StageManager::SetStage2(const int& blockWidth, const int& blockHeight)
{
	this->tutorial->ElectricInitialize();
	stageWidth = blockWidth;
	selectStage = STAGE::STAGE2;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//ここでプレイヤーの位置指定
	playerPos[0][0] = true;

	//プレイヤーの制限回数の上限を設定
	
	SetisCountReset(selectStage);

	//形の設定
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::Electric,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GEAR,Form::GEAR,Form::BUTTON,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::LOCKED,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};

	//行列をセット
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}

void StageManager::SetStage3(const int& blockWidth, const int& blockHeight)
{
	stageWidth = blockWidth;
	selectStage = STAGE::STAGE3;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//ここでプレイヤーの位置指定
	playerPos[0][0] = true;

	//プレイヤーの制限回数の上限を設定

	SetisCountReset(selectStage);

	//形の設定
	forms_ =
	{
		{Form::GEAR,Form::BLOCK,Form::NONE,Form::BUTTON,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::NONE,Form::GEAR,Form::NONE,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GEAR,Form::NONE,Form::NONE,Form::GEAR,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::NONE,Form::BUTTON,Form::Electric,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};

	//行列をセット
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}

void StageManager::ResetStage()
{
	blockManager_->ResetBlock();
}

void StageManager::SelectStage(int stageNum)
{
	switch(stageNum)
	{
	case STAGE::TUTORIAL:
		SetTutorial(13, 13);
		break;
	case STAGE::STAGE1:
		SetStage1(13, 13);
		break;
	case STAGE::STAGE2:
		SetStage2(13, 13);
		break;
	case STAGE::STAGE3:
		SetStage3(13, 13);
		break;
	}
}

void StageManager::SetisCountReset(int stageNum)
{
	//フラグをオンにする
	if(isCountReset[stageNum] == false)
	{
		for(int i = 0; i < stageMax; i++)
		{
			if(i == stageNum)
			{
				isCountReset[i] = true;
			}
			else
			{
				isCountReset[i] = false;
			}
		}
	}

}
