#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"
#include "ConnectingEffect.h"

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
		NONE,			//�����Ȃ����
	};

	enum Action
	{
		Rotate,		//��]���Ă���Ƃ�
		Overlap,	//�d�Ȃ��Ă���Ƃ�
		Connect,	//�q�����Ă���Ƃ�
		None,		//�������Ă��Ȃ��Ƃ�
	};

private:


	ConnectingEffectManager* connectEM;

	//Object* objcet_ = nullptr;

	Model* normal_ = nullptr;
	Model* button_ = nullptr;
	Model* goal_ = nullptr;
	Model* socket_ = nullptr;


	//�e�N�X�`���n���h��
	//UINT64 textureHandle_[10];

	const float scaleTmp = 1.8f;

	//�u���b�N�̑傫��
	Vec3 scale_;

	//���W
	Vec3 pos_;

	XMFLOAT4 color;

	int count = 0;


public:

	~Block();

	Object draw[10];
	DebugText* debugText_ = nullptr;

	void Initialize(ConnectingEffectManager* connectEM,
		Model* normal, Model* button, Model* goal, Model* Socket);

	void Updata(Vec3 pos = { 0,0,0 });

	void Draw(Camera* camera, UINT64* texhandle, int form, Action action);

	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	//�Փˎ��ɌĂ΂��
	void OnCollision(Collider& collider)override;
	//��ƓG�̔���p
	void OnCollision2(Collider& collider)override;

	//�Q�b�^�[
	const Vec3 GetPos(Vec3 pos) const { return pos_; };
	const Vec3 GetScale(Vec3 scale) const { return scale_; };

	//const WorldMat GetWorldTransform() { return worldTransform_; };


	//�Z�b�^�[
	void SetWorldPos(Vec3& pos);
	//�Q�b�^�[
	float GetRadius() { return radius_; };
};

