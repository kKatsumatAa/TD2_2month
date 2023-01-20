#pragma once
#include "Block.h"
#include "Tutorial.h"
#include <vector>
#include <string>
#include <sstream>
#include "GoalEffect.h"


class BlockManager
{

	using Form = Block::Form;
	using Action = Block::Action;

public:

	~BlockManager();

	//������
	void Initialize(ConnectingEffectManager* connectEM, Tutorial* tutorial, CameraManager* cameraM, GoalEffect* goalEffect,
		Model* normal, Model* button, Model* goal, Model* Socket);

	//�X�V
	void Update();

	//�`��
	void Draw(Camera* camera);

	//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
	bool CheckPlayerOnBlock(Vec3 pos);

	//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos�����)
	bool GetPosIsBlock(Vec3 pos);

	//�{�^�������邩�ǂ���
	bool GetPosIsButton(Vec3 pos);

	//�ŏ��Ɍq���{�^����������u���b�N����ɓo�^����֐�
	void RegistAxisButton(const Vec3& pos);

	//�u���b�N���m��Ȃ��X�V�֐�
	void UpdateConnect(Vec3 pos);

	//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
	bool CheckAxisButton(Vec3 pos);

	//�q����Ă���u���b�N��S���������
	void ReleseConectedBlock();

	//�L�[�{�[�h�ɂ���ĉ�]
	void UpdateRotate(Vec3& rotatePos);

	//��]����������
	bool GetIsRollingLeftorRight();

	//�S�[�������邩�ǂ���
	bool GetIsGoal(Vec3& Pos, bool isPlayer = false);

	//�d�Ȃ������̏���
	void UpdateOverlap();

	//�d�Ȃ��Ă����u���b�N����ɖ߂�����
	void RepositBlock();

	//�u���b�N�ƃu���b�N�̓����蔻��
	bool CollisionBlockToBlock(Vec3 blockPos, Vec3 pos);

	//�i�ސ�ɏd�Ȃ��Ă���u���b�N�����邩�ǂ���
	//bool GetIsOverlapBlock(Vec3 pos);

	//�u���b�N���W�ǂݍ���
	void LoadBlockPosData();
	void UpdateBlockPos();
	//�u���b�N�̔����֐�
	void BlockPop(Vec3 pos);

	//�u���b�N�̃��Z�b�g
	void ResetBlock();

	//��]���I������Ƃ��Ƀp�[�e�B�N������������
	void GenerateParticleTurnBlock();

	//�`��p��Y���W����炷�֐�
	void ChangePosY();

	//�X�e�[�W��Z�b�g����
	void SetStage(const int& stageWidth, const int& stageHeight ,std::vector<std::vector<WorldMat>>& worldmats, std::vector<std::vector<Form>>& forms);

	//�Q�b�^�[
	float GetRadius() { return blockRadius_; };

public:

	static const int blockWidth = 13;
	static const int blockHeight = 13;
	//���a
	const float blockRadius_ = 1.8f;


private:
	CameraManager* cameraM;

	Tutorial* tutorial;

	GoalEffect* goalEffect;

	UINT64 texhandle[10];

	const int width = 13;

	Model* model_ = nullptr;

	std::vector<Vec3> goalCameraPoses;

	//�u���b�N�̓񎟌��z��
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	Block* block_;
	std::vector<std::vector<Block*>> blocks_;

	//�e�u���b�N�p�̃��[���h�g�����X�t�H�[��
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	//��ԕω��ϐ�
	//���݂̌`
	Form form_[blockWidth][blockHeight];

	Form formTmp_[blockWidth][blockHeight] = {
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
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


	//��]����O�̌`
	Form beforeForm_[blockWidth][blockHeight] = { Form::BLOCK };

	Action action_[blockWidth][blockHeight] = { Action::None };

	//���ɂȂ��Ă��邩�ǂ���
	bool isAxis_[blockWidth][blockHeight];

	//���ɂȂ��Ă���u���b�N�̍��W
	Vec3 axis_pos_;

	//�I�����Ă��邩�ǂ���
	int isCount;

	//�u���b�N�̑傫��
	Vec3 scale_;


	//�O�̃t���[���ɂ����đI��p�̃u���b�N�̏���ۑ����Ă����ϐ�
	int prevBlockX;
	int prevBlockY;

	//�I��J�[�\���̃N�[���^�C�}�[�̐ݒ莞��
	static const int32_t kSelectTime = 17;

	//�I��J�[�\���̃N�[���^�C�}�[
	int32_t selectTimer_ = kSelectTime;

	//�I���Ԃɂ��Ă������
	bool changedAction_;
	bool isChanged_;

	Vec3 transforms[blockWidth][blockHeight];

	Object draw[10];

	//��]

	bool isRightRolling;
	bool isLeftRolling;

	int rotateCount;
	const int rotateCountMax = 40;

	float angle_;

	Vec3 distancePos[blockWidth][blockHeight];
	Vec3 distancePosPlayer;

	ConnectingEffectManager* connectEM;

	int effectCount = 0;
	const int effectCountMax = 10;

	//��r�p�ɕۑ����Ă����ϐ�
	Vec3 comparisonPos[blockWidth][blockHeight];

	//�t�@�C���ǂݍ��ݗp�̕ϐ�
	std::stringstream blocksPos;

	//�ҋ@���t���O
	bool isWaitBlock;
	//�ҋ@���^�C�}�[
	int32_t blockWaitTimer;

	int isOverLap_[blockWidth][blockHeight];

	//��]����O�̌`��
	Form beforeTurn_[blockWidth][blockHeight];

	bool isUp[blockWidth][blockHeight];

	//����ۑ��������ǂ���
	bool isTurn[blockWidth][blockHeight];

	//読み込んだブロックの形
	Form loadForm_;
	std::vector<std::vector<Block::Form>> loadForms_;
	//読み込んだブロックの初期座標
	std::vector<std::vector<WorldMat>> loadWorldmats_;

	//読み込んだステージの長さ
	float stageWidth_;
	float stageHeight_;

};

