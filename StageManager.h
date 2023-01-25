#pragma once
#include "BlockManager.h"

enum STAGE
{
	TUTORIAL,
	STAGE1,
	STAGE2,
	STAGE3,
};

class StageManager
{
	using Form = Block::Form;

public:

	//������
	void Initialize(BlockManager* blockManager);

	//�X�V
	void Update();

	//���[���h�s��Z�b�g
	void SetWorldMat(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// �`���[�g���A�����Z�b�g
	/// </summary>
	void SetTutorial(const int& blockWidth, const int& blockHeight);
	/// <summary>
	/// �X�e�[�W1���Z�b�g
	/// </summary>
	void SetStage1(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// �X�e�[�W2���Z�b�g
	/// </summary>
	void SetStage2(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// �X�e�[�W2���Z�b�g
	/// </summary>
	void SetStage3(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// �X�e�[�W�����Z�b�g
	/// </summary>
	void ResetStage();

	/// <summary>
	/// �����ɂ���ăX�e�[�W�I��
	/// </summary>
	/// <param name="stageNum"></param>
	void SelectStage(int stageNum);

	//�`���[�g���A���X�e�[�W�̒���
	static const int TutorialWidth = 13;
	static const int TutorialHeight = 13;
	//�X�e�[�W1�̒���
	static const int stage1Width = 13;
	static const int stage1Height = 13;

	//�X�e�[�W��
	static const int stageMax = 4;
	static int selectStage;

	//�O���ŎQ�Ƃ��邽�߂�
	int stageWidth = 0;

private:

	//�X�e�[�W��
	enum Stage
	{
		Tutorial,
		Stage1,

	};

	BlockManager* blockManager_;

	//�e�u���b�N�p�̃��[���h�g�����X�t�H�[��
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	Form form_;
	std::vector<std::vector<Block::Form>> forms_;


};

