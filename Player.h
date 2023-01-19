#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"
#include "BlockManager.h"
#include "PlayerSocket.h"


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
class StateNormalMoveP : public PlayerState
{
private:
	int countE = 0;

	Shake shake;

	int effectCount = 0;
	int effectCountTmp = 60;

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//�ړ���
class StateMoveP : public PlayerState
{
private:
	int count = 0;
	const int countMax = 20;



public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//-----------------------------------------------
//�������Ă��Ȃ�
class StateNormalConTurP : public PlayerState
{
private:
	int countE = 0;

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//�q���ł�
class StateConnectP : public PlayerState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//�񂵂Ă�
class StateTurnP : public PlayerState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//--------------------------------------------------------

enum BUFFERED_INPUT_ARROW
{
	NONE,
	RIGHT,
	DOWN,
	LEFT,
	UP
};

class Player : public Collider
{
private:
	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	UINT64 textureHandle[10];

	//���
	PlayerState* stateMove = nullptr;
	PlayerState* stateConnectTurn = nullptr;

	const float scaleTmp = 1.8f;



	/*Tutorial* tutorial;*/
public:
	float posYTmp = 0;
	float posXTmp = 0;


	//�O������Z�b�g����u���b�N���邩�瓮����
	bool isMove = false;
	bool isWantToMove = false; //�O���ŉ����������Q�Ƃ��Ă��炤
	bool isMoveNow = false;	//�����Ă��邩
	//�O������Z�b�g����u���b�N���邩��q����t���O
	bool isConnect = false;
	bool isWantToConnect = false;  //�O���ŉ����������Q�Ƃ��Ă��炤
	bool isCennectNow = false;	//�����Ă��邩
	//�V�u���b�N��]����t���O
	bool isTurn = false;
	bool isisWantToTurn = false;  //�O���ŉ����������Q�Ƃ��Ă��炤
	bool isTurnNow = false;  //�����Ă��邩

	bool isGoal = false;

	float moveDistance;
	//�i�݂����ꏊ
	Vec3 moveEndPos;
	Vec3 moveStartPos;

	//��s���͗p
	bool bufferedPushSpace = false;
	int bufferedKeyArrow = BUFFERED_INPUT_ARROW::NONE;

	Object draw[10];
	DebugText* debugText_ = nullptr;

	BlockManager* blockM;

	PlayerSocket* playerSocket;

	ConnectingEffect2Manager* connectE2M;

	Tutorial* tutorial;


		void ChangeStateTurnConnect(PlayerState* state);
	void ChangeStateMove(PlayerState* state);

	void Initialize(float moveDistance, BlockManager* blockM, PlayerSocket* playerSocket,
		ConnectingEffect2Manager* connectE2M, Tutorial* tutorial, Model* model, DebugText* debugText_/*,Tutorial* tutorial = nullptr*/);
	void Update();
	void Draw(Camera* camera);
	void DrawSprite();

	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	void Reset();

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	//
	void OnCollision(Collider& collider)override;
	//
	void OnCollision2(Collider& collider)override;
};


