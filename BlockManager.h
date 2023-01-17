#pragma once
#include "Block.h"
#include "Tutorial.h"
#include <vector>
#include <string>
#include <sstream>


class BlockManager 
{

	using Form = Block::Form;
	using Action = Block::Action;

public:

	~BlockManager();

	//初期化
	void Initialize(ConnectingEffectManager* connectEM, Tutorial* tutorial,Camera * camera,
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
	bool GetPosIsButton(Vec3 pos);

	//最初に繋ぐボタンを押したブロックを軸に登録する関数
	void RegistAxisButton(const Vec3& pos);

	//ブロック同士をつなぐ更新関数
	void UpdateConnect(Vec3 pos);

	//繋ぐ際に離したところが軸以外のボタンかどうか
	bool CheckAxisButton(Vec3 pos);

	//繋がれているブロックを全部解除する
	void ReleseConectedBlock();

	//キーボードによって回転
	void UpdateRotate(Vec3& rotatePos);

	//回転中だったら
	bool GetIsRollingLeftorRight();

	//ゴールがあるかどうか
	bool GetIsGoal(Vec3& Pos);

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

public:

	static const int blockWidth = 13;
	static const int blockHeight = 13;
	//半径
	const float blockRadius_ = 1.8f;


private:
	Tutorial* tutorial;

	Camera* camera;

	UINT64 texhandle[10];

	const int width = 13;

	Model* model_ = nullptr;
	
	//ブロックの二次元配列
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	Block* block_;
	std::vector<std::vector<Block*>> blocks_;

	//各ブロック用のワールドトランスフォーム
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	//ワールド変換データ(こっちまだ使ってないけど消さないでほしい)
	/*WorldMat worldTransform_[blockWidth][blockHeight];
	WorldMat preWorldTransform_[blockWidth][blockHeight];*/

	//状態変化変数
	//現在の形
	Form form_[blockWidth][blockHeight] = { 
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

	
	//前のフレームにおいて選択用のブロックの情報を保存しておく変数
	int prevBlockX;
	int prevBlockY;

	//選択カーソルのクールタイマーの設定時間
	static const int32_t kSelectTime = 17;

	//選択カーソルのクールタイマー
	int32_t selectTimer_ = kSelectTime;

	//選択状態にしてもいいか
	bool changedAction_;
	bool isChanged_;

	Vec3 transforms[blockWidth][blockHeight];

	Object draw[10];

	//回転

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

	//比較用に保存しておく変数
	Vec3 comparisonPos[blockWidth][blockHeight];

	//ファイル読み込み用の変数
	std::stringstream blocksPos;

	//待機中フラグ
	bool isWaitBlock;
	//待機中タイマー
	int32_t blockWaitTimer;

	int isOverLap_[blockWidth][blockHeight];

};

