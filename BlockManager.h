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

	//初期化
	void Initialize(ConnectingEffectManager* connectEM, Tutorial* tutorial, CameraManager* cameraM, GoalEffect* goalEffect,
		Model* normal, Model* button, Model* goal, Model* Socket);

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

	//プレイヤーがブロックの上にいるかどうか
	bool CheckPlayerOnBlock(Vec3 pos);

	//ステージの関数で先にブロックあるか判定(endPosを引数)
	bool GetPosIsBlock(Vec3 pos);

	//ボタンがあるかどうか
	bool GetPosIsGear(Vec3 pos);

	//最初に繋ぐボタンを押したブロックを軸に登録する関数
	void RegistAxisGear(const Vec3& pos);

	//ブロック同士をつなぐ更新関数
	void UpdateConnect(Vec3 pos);

	//繋ぐ際に離したところが軸以外のボタンかどうか
	bool CheckAxisGear(Vec3 pos);

	//繋がれているブロックを全部解除する
	void ReleseConectedBlock();

	//キーボードによって回転
	void UpdateRotate(Vec3& rotatePos);

	//回転中だったら
	bool GetIsRollingLeftorRight();

	//ゴールがあるかどうか
	bool GetIsGoal(Vec3& Pos, bool isPlayer = false);

	//重なった時の処理
	void UpdateOverlap();

	//重なっていたブロックを元に戻す処理
	void RepositBlock();

	//ブロックとブロックの当たり判定
	bool CollisionBlockToBlock(Vec3 blockPos, Vec3 pos);

	//進む先に重なっているブロックがあるかどうか
	//bool GetIsOverlapBlock(Vec3 pos);

	//ブロック座標読み込み
	void LoadBlockPosData();
	void UpdateBlockPos();
	//ブロックの発生関数
	void BlockPop(Vec3 pos);

	//ブロックのリセット
	void ResetBlock();

	//回転し終わったときにパーティクル発生させる
	void GenerateParticleTurnBlock();

	//描画用にY座標をずらす関数
	void UpPosY();

	void DownPosY();


	//読み込んだステージをセットする関数
	void SetStage(const int& stageWidth, const int& stageHeight ,std::vector<std::vector<WorldMat>>& worldmats, std::vector<std::vector<Form>>& forms);

	//半径を渡す関数
	float GetRadius() { return blockRadius_; };

	//ボタンと重なった時にゴールを出す仕組み
	void AppearGoal();

public:

	static const int blockWidth = 13;
	static const int blockHeight = 13;
	//半径
	const float blockRadius_ = 1.8f;


private:
	CameraManager* cameraM;

	Tutorial* tutorial;

	GoalEffect* goalEffect;

	UINT64 texhandle[10];

	const int width = 13;

	Model* model_ = nullptr;

	std::vector<Vec3> goalCameraPoses;

	//ブロックの二次元配列
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	Block* block_;
	std::vector<std::vector<Block*>> blocks_;

	//各ブロック用のワールドトランスフォーム
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	//状態変化変数
	//現在の形
	Form form_[blockWidth][blockHeight];

	Form formTmp_[blockWidth][blockHeight] = {
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::GEAR,Form::GEAR,Form::NONE,Form::GEAR,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::BUTTON,Form::BUTTON,Form::NONE,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::GEAR,Form::BLOCK,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::GEAR,Form::GEAR,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::BLOCK,Form::BLOCK,Form::GOAL,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
		{Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,Form::NONE,},
	};


	//回転する前の形
	Form beforeForm_[blockWidth][blockHeight] = { Form::BLOCK };

	Action action_[blockWidth][blockHeight] = { Action::None };

	//軸になっているかどうか
	bool isAxis_[blockWidth][blockHeight];

	//軸になっているブロックの座標
	Vec3 axis_pos_;

	//選択されているかどうか
	int isCount;

	//ブロックの大きさ
	Vec3 scale_;

	Vec3 transforms[blockWidth][blockHeight];

	Object draw[10];

	//右回転しているかどうか
	bool isRightRolling;
	//左回転
	bool isLeftRolling;

	int rotateCount;
	const int rotateCountMax = 40;

	float angle_;

	Vec3 distancePos[blockWidth][blockHeight];
	Vec3 distancePosPlayer;

	ConnectingEffectManager* connectEM;

	int effectCount = 0;
	const int effectCountMax = 10;

	//比較用に保存しておく変数
	Vec3 comparisonPos[blockWidth][blockHeight];

	//ファイル読み込み用の変数
	std::stringstream blocksPos;

	//待機中フラグ
	bool isWaitBlock;
	//待機中タイマー
	int32_t blockWaitTimer;
	//各ブロックの当たり判定保存用
	int isOverLap_[blockWidth][blockHeight];

	//回転する前の形状
	Form beforeTurn_[blockWidth][blockHeight];

	bool isUp[blockWidth][blockHeight];
	bool isDown[blockWidth][blockHeight];

	//回転したかどうか
	bool isTurn[blockWidth][blockHeight];

	//読み込んだブロックの形
	Form loadForm_;
	std::vector<std::vector<Block::Form>> loadForms_;
	//読み込んだブロックの初期座標
	std::vector<std::vector<WorldMat>> loadWorldmats_;

	//読み込んだステージの長さ
	float stageWidth_;
	float stageHeight_;

	//上がる前の座標
	float beforeTransY[blockWidth][blockHeight];

	//ボタンが押されたかどうか(配列にしているのは複数ボタンに対応するため)
	bool isPushed[blockWidth][blockHeight];
	int pushedCount_;

	//ゴールの位置を保存する配列
	bool isGoal_[blockWidth][blockHeight];
	//読み込んだボタンの数から必要な押す数を入れておく
	int needGoalCount;

	//ゴールが出現しているかどうか
	bool isPopGoal;

};

