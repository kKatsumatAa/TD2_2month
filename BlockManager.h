#pragma once
#include "Block.h"
#include <vector>
#include <string>



class BlockManager 
{

	using Form = Block::Form;
	using Action = Block::Action;

public:

	~BlockManager();

	//������
	void Initialize(ConnectingEffectManager* connectEM,Camera * camera);

	//�X�V
	void Update();

	//�`��
	void Draw(Camera* camera);

	//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
	bool CheckPlayerOnBlock(Vec3 pos);

	//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)
	bool GetPosIsBlock(Vec3 pos);

	//�{�^�������邩�ǂ���
	bool GetPosIsButton(Vec3 pos);

	//�ŏ��Ɍq���{�^�����������u���b�N�����ɓo�^����֐�
	void RegistAxisButton(const Vec3& pos);

	//�u���b�N���m���Ȃ��X�V�֐�
	void UpdateConnect(Vec3 pos);

	//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
	bool CheckAxisButton(Vec3 pos);

	//�q����Ă���u���b�N��S����������
	void ReleseConectedBlock();

	//�L�[�{�[�h�ɂ���ĉ�]
	void UpdateRotate(Vec3& rotatePos);

	//��]����������
	bool GetIsRollingLeftorRight();

	//�S�[�������邩�ǂ���
	bool GetIsGoal(Vec3& Pos);


public:

	static const int blockWidth = 13;
	static const int blockHeight = 13;
	//���a
	const float blockRadius_ = 1.8f;


private:
	Camera* camera;

	UINT64 texhandle[10];

	const int width = 13;

	Model* model_ = nullptr;
	
	//�u���b�N�̓񎟌��z��
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	Block* block_;
	std::vector<std::vector<Block*>> blocks_;

	//�e�u���b�N�p�̃��[���h�g�����X�t�H�[��
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	//���[���h�ϊ��f�[�^(�������܂��g���ĂȂ����Ǐ����Ȃ��łق���)
	/*WorldMat worldTransform_[blockWidth][blockHeight];
	WorldMat preWorldTransform_[blockWidth][blockHeight];*/

	//��ԕω��ϐ�
	Form form_[blockWidth][blockHeight] = { Form::BLOCK };

	Action action_[blockWidth][blockHeight] = { Action::None };

	//���ɂȂ��Ă��邩�ǂ���
	bool isAxis_[blockWidth][blockHeight];

	//���ɂȂ��Ă���u���b�N�̍��W
	Vec3 axis_pos_;

	//�I������Ă��邩�ǂ���
	int isCount;

	//�u���b�N�̑傫��
	Vec3 scale_;

	
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
	const int effectCountMax = 5;
};

