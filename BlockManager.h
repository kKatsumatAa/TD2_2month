#pragma once
#include "Block.h"
#include <vector>

class BlockManager 
{

	using Form = Block::Form;
	using Action = Block::Action;

public:

	~BlockManager();

	//������
	void Initialize(Model* model, DebugText* debugText_);

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
	bool CheckPlayerOnBlock(Vec3 pos);

	//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)
	bool GetPosIsBlock(Vec3 pos);

	//�{�^�������邩�ǂ���
	bool GetPosIsButton(Vec3 pos);

	//�ŏ��Ɍq���{�^�����������u���b�N�����ɓo�^����֐�
	void RegistAxisButton(const Vec3& pos, bool isConnect);

	//�u���b�N���m���Ȃ��X�V�֐�
	void UpdateConnect(Vec3 pos, bool isConnect);

	//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
	bool CheckAxisButton(Vec3 pos, bool isConnect);

	//�q����Ă���u���b�N��S����������
	void ReleseConectedBlock();

	//�L�[�{�[�h�ɂ���ĉ�]
	//�L�[�{�[�h���֐����Ŏ擾
	void UpdateRotatePlayerBlock(Vec3& rotatePos, bool isRota);

public:

	static const int blockWidth = 13;
	static const int blockHeight = 13;

private:
	
	Model* model_ = nullptr;
	
	//�u���b�N�̓񎟌��z��
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	//std::vector< std::unique_ptr<std::vector<Block>> > blocks_;

	Block* blocks_[blockWidth][blockHeight];

	//���[���h�ϊ��f�[�^
	WorldMat worldTransform_[blockWidth][blockHeight];
	WorldMat preWorldTransform_[blockWidth][blockHeight];

	//��ԕω��ϐ�
	Form form_[blockWidth][blockHeight] = { Form::BLOCK };

	Action action_[blockWidth][blockHeight] = { Action::None };

	//�}�b�v�`�b�v�ŊǗ�����t���O
	////��]���Ă��邩�ǂ���
	//bool isRotate_[blockWidth][blockHeight];
	////�d�Ȃ��Ă��邩�ǂ���
	//bool isOverlap_[blockWidth][blockHeight];
	////�q�����Ă��邩�ǂ���
	//bool isConnect_[blockWidth][blockHeight];
	//���ɂȂ��Ă��邩�ǂ���
	bool isAxis_[blockWidth][blockHeight];

	//�I������Ă��邩�ǂ���
	int isCount;

	//�u���b�N�̑傫��
	Vec3 scale_;

	//���a
	float radius_;

	//�O�̃t���[���ɂ����đI��p�̃u���b�N�̏���ۑ����Ă����ϐ�
	int prevBlockX;
	int prevBlockY;

	//�I���J�[�\���̃N�[���^�C�}�[�̐ݒ莞��
	static const int32_t kSelectTime = 17;

	//�I���J�[�\���̃N�[���^�C�}�[
	int32_t selectTimer_ = kSelectTime;

	//�I����Ԃɂ��Ă�������
	bool changedAction_;
	bool isChanged_;

};

