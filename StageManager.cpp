#include "StageManager.h"

using namespace std;

int StageManager::selectStage = STAGE::TUTORIAL;

void StageManager::Initialize(BlockManager* blockManager)
{
	//�C���X�^���X�̐���
	//blockManager_ = std::make_unique<BlockManager>();
	blockManager_ = blockManager;

	int tutorialWidth = 13;
	int tutorialHeight = 13;

	//�x�N�^�z��ɗv�f<���[���h�s��>��ǉ�
	for (int i = 0; i < tutorialWidth; i++)
	{
		//�u���b�N�^�����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		worldmats_.push_back(vector<WorldMat>());

		for (int j = 0; j < tutorialHeight; j++)
		{
			//�u���b�N�̗v�f��ǉ�
			worldmats_[i].push_back(worldmat_);
		}
	}
	
	//�x�N�^�z��ɗv�f<�`>��ǉ�
	for (int i = 0; i < tutorialWidth; i++)
	{
		//�u���b�N�^�����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		forms_.push_back(vector<Block::Form>());

		for (int j = 0; j < tutorialHeight; j++)
		{
			//�`�̗v�f��ǉ�
			forms_[i].push_back(form_);
		}
	}

	////�����ōŏ��̃X�e�[�W��ݒ�
	//SetTutorial(13,13);
}

void StageManager::Update()
{

}

void StageManager::SetWorldMat(const int& blockWidth, const int& blockHeight)
{
	//�u���b�N�̔��a
	float blockRadius_ = blockManager_->GetRadius();

	//�c���A����
	int tutorialWidth = blockWidth;
	int tutorialHeight = blockHeight;

	//�����A�c���̗v�f����ύX

	//�c��
	worldmats_.resize(tutorialWidth);
	//����
	for (int i = 0; i < tutorialWidth; i++)
	{
		worldmats_[i].resize(tutorialHeight);
	}
	//�c��
	forms_.resize(tutorialWidth);
	//����
	for (int i = 0; i < tutorialWidth; i++)
	{
		forms_[i].resize(tutorialHeight);
	}

	//���[���h�s��̐ݒ�
	for (int i = 0; i < tutorialWidth; i++)
	{
		for (int j = 0; j < tutorialHeight; j++)
		{
			worldmats_[i][j].scale = { blockRadius_ ,blockRadius_,blockRadius_ };

			//�u���b�N�̍��W��ݒ�
			if (i >= 0)
			{
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);
			}
			if (j >= 0)
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
	selectStage = STAGE::TUTORIAL;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//�`�̐ݒ�
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::Electric,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GEAR,Form::GEAR,Form::NONE,Form::GEAR,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BUTTON,Form::BUTTON,Form::NONE,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::GEAR,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::GEAR,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};
	
	//�s����Z�b�g
	blockManager_->SetStage(blockWidth, blockHeight,worldmats_,forms_);
}

void StageManager::SetStage1(const int& blockWidth, const int& blockHeight)
{
	selectStage = STAGE::STAGE1;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//�`�̐ݒ�
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GEAR,Form::GEAR,Form::NONE,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::GEAR,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};

	//�s����Z�b�g
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}

void StageManager::SetStage2(const int& blockWidth, const int& blockHeight)
{
	selectStage = STAGE::STAGE2;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//�`�̐ݒ�
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::GEAR,Form::NONE},
		{Form::GEAR,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE},
	};

	//�s����Z�b�g
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}

void StageManager::SetStage3(const int& blockWidth, const int& blockHeight)
{
	selectStage = STAGE::STAGE3;

	ResetStage();
	SetWorldMat(blockWidth, blockHeight);

	//�`�̐ݒ�
	forms_ =
	{
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::GEAR,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};

	//�s����Z�b�g
	blockManager_->SetStage(blockWidth, blockHeight, worldmats_, forms_);

}

void StageManager::ResetStage()
{
	blockManager_->ResetBlock();
}

void StageManager::SelectStage(int stageNum)
{
	switch (stageNum)
	{
	case STAGE::TUTORIAL:
		SetTutorial(13, 13);
		break;
	case STAGE::STAGE1:
		SetStage1(13, 13);
		break;
	case STAGE::STAGE2:
		SetStage2(2, 10);
		break;
	case STAGE::STAGE3:
		SetStage3(13, 13);
		break;
	}
}
