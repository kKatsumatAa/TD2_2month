#pragma once
#include "BlockManager.h"

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


	//�`���[�g���A���X�e�[�W�̒���
	static const int TutorialWidth = 13;
	static const int TutorialHeight = 13;
	//�X�e�[�W1�̒���
	static const int stage1Width = 13;
	static const int stage1Height = 13;


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

