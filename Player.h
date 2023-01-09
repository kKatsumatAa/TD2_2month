#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"

class Player;

class PlayerState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update() = 0;
	virtual void Draw(Camera* camera, Model* model) = 0;
};

//�������Ă��Ȃ�
class StateNormalP : public PlayerState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//�W�����v�̂�
class StateConnectP : public PlayerState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//�W�����v�U����
class StateTurnP : public PlayerState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//--------------------------------------------------------

class Player : public Collider
{
private:
	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	UINT64 textureHandle[10];


	//���
	PlayerState* state = nullptr;

	const float scaleTmp = 1.8f;

	/*Tutorial* tutorial;*/
public:
	//
	Camera* camera;
	Object draw[10];
	DebugText* debugText_ = nullptr;


	void ChangeState(PlayerState* state);

	void Initialize(Model* model, DebugText* debugText_, Camera* camera	/*,Tutorial* tutorial = nullptr*/);
	void Update();
	void Draw(Camera* camera);
	void DrawSprite();

	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	//
	void OnCollision(Collider& collider)override;
	//
	void OnCollision2(Collider& collider)override;
};


