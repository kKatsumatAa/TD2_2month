#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"

//�u���b�N
class Block : public Collider
{

public:
	//�`�ԃt�F�[�Y
	enum Form
	{
		BLOCK = 0,		//�u���b�N���
		GEAR,			//�M�A
		BUTTON,			//�{�^��
		LOCKED,			//�Œ�u���b�N
		GOAL,			//�S�[��
		None,			//�����Ȃ����
	};

	enum Action
	{
		Rotate,		//��]���Ă���Ƃ�
		Overlap,	//�d�Ȃ��Ă���Ƃ�
		Connect,	//�q�����Ă���Ƃ�
		None,		//�������Ă��Ȃ��Ƃ�
	};

private:

	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	UINT64 textureHandle_[10];

	const float scaleTmp = 1.8f;

	bool isRotate_;		//��]���Ă��邩�ǂ���
	bool isOverlap_;	//�d�Ȃ��Ă��邩�ǂ���
	bool isConnect_;	//�q�����Ă��邩�ǂ���
	bool isAxis_;		//���ɂȂ��Ă��邩�ǂ���

	//�u���b�N�̑傫��
	Vec3 scale_;
	//���a
	float radius_;

	//���W
	Vec3 pos_;

	//�ړ�����
	float moveDistance_;

	

public:

	Object draw[10];
	DebugText* debugText_ = nullptr;

	void Initialize(Model* model, DebugText* debugText_);

	void Updata();

	void Draw(Camera* camera);

	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	//�Փˎ��ɌĂ΂��
	void OnCollision(Collider& collider)override;
	//��ƓG�̔���p
	void OnCollision2(Collider& collider)override;
	
	//�Q�b�^�[
	const Vec3 GetPos(Vec3 pos) const { return pos_; };
	const Vec3 GetScale(Vec3 scale) const { return scale_; };

	const WorldMat GetWorldTransform() { return worldTransform_; };


	//�Z�b�^�[
};

