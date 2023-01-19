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

//何もしていない
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

//移動中
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
//何もしていない
class StateNormalConTurP : public PlayerState
{
private:
	int countE = 0;

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//繋いでる
class StateConnectP : public PlayerState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//回してる
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
	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	UINT64 textureHandle[10];

	//状態
	PlayerState* stateMove = nullptr;
	PlayerState* stateConnectTurn = nullptr;

	const float scaleTmp = 1.8f;



	/*Tutorial* tutorial;*/
public:
	float posYTmp = 0;
	float posXTmp = 0;


	//外部からセットするブロックあるから動ける
	bool isMove = false;
	bool isWantToMove = false; //外部で何したいか参照してもらう
	bool isMoveNow = false;	//今しているか
	//外部からセットするブロックあるから繋げるフラグ
	bool isConnect = false;
	bool isWantToConnect = false;  //外部で何したいか参照してもらう
	bool isCennectNow = false;	//今しているか
	//〃ブロック回転するフラグ
	bool isTurn = false;
	bool isisWantToTurn = false;  //外部で何したいか参照してもらう
	bool isTurnNow = false;  //今しているか

	bool isGoal = false;

	float moveDistance;
	//進みたい場所
	Vec3 moveEndPos;
	Vec3 moveStartPos;

	//先行入力用
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

	//衝突を検出したら呼び出す（コールバック関数）
	//
	void OnCollision(Collider& collider)override;
	//
	void OnCollision2(Collider& collider)override;
};


