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
		Electric,		//�d�C�o�̓u���b�N
		NONE,			//�����Ȃ����
	};

	enum Action
	{
		Connect,	//�q�����Ă���Ƃ�
		None,		//�������Ă��Ȃ��Ƃ�
	};

private:

	ConnectingEffectManager* connectEM;

	Model* normal_ = nullptr;
	Model* locked_ = nullptr;
	Model* button_ = nullptr;
	Model* goal_ = nullptr;
	Model* socket_ = nullptr;
	Model* disconnectedBlock_ = nullptr;
	Model* disconnectedButton_ = nullptr;
	Model* disconnectedSocketBlock_ = nullptr;
	Model* electricBlock_ = nullptr;
	Model* doorGoalClosed_ = nullptr;
	Model* overlapBlock_ = nullptr;
	//�S�[�����o�Ă���O�̘g�g��
	Model* beforePopGoal_ = nullptr;

	const float scaleTmp = 1.8f;

	//�u���b�N�̑傫��
	Vec3 scale_;

	//���W
	Vec3 pos_;

	XMFLOAT4 color;
	
	WorldMat goalMat_;

	//�����ł����Ă��Ă��邯�ǁA���ŕK�v�Ȃ̂�
	int count = 0;

	bool isGoalElec = false;


public:

	~Block();

	Object draw[20];

	void Initialize(ConnectingEffectManager* connectEM,
		Model* normal, Model* locked, Model* goal, Model* Socket, Model* button, Model* disconnectedBlock,
		Model* disconnectedButton, Model* disconnectedSocketBlock, Model* electricBlock, Model* doorGoalClosed,
		Model* overlapBlock,Model* beforePopGoal);

	void Updata(Vec3 pos, int form, Action action, bool isElec, int count, WorldMat goalMat, bool isPosGoal);
	void SetAlpha(float blockAlpha);
	void SetColor(Vec3 blockColor);

	void Draw(Camera* camera, UINT64* texhandle, int form, Action action, bool isElec, bool isPushed, int count, float popAlpha,bool isPosGoal,bool isPopGoal);
	
	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	//�Q�b�^�[
	const Vec3 GetPos(Vec3 pos) const { return pos_; };
	const Vec3 GetScale(Vec3 scale) const { return scale_; };
	float GetRadius() { return radius_; };

	//�Z�b�^�[
	void SetWorldPos(Vec3& pos);
	void SetScale(const Vec3& scale);
};

