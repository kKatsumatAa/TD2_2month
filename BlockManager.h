#pragma once
#include "Block.h"
#include <vector>

class BlockManager 
{

	using Form = Block::Form;
	using Action = Block::Action;

public:

	~BlockManager();

	//初期化
	void Initialize(Model* model, DebugText* debugText_);

	//更新
	void Update();

	//描画
	void Draw();

	//プレイヤーがブロックの上にいるかどうか
	bool CheckPlayerOnBlock(Vec3 pos);

	//ステージの関数で先にブロックあるか判定(endPosを引数)
	bool GetPosIsBlock(Vec3 pos);

	//ボタンがあるかどうか
	bool GetPosIsButton(Vec3 pos);

	//最初に繋ぐボタンを押したブロックを軸に登録する関数
	void RegistAxisButton(const Vec3& pos, bool isConnect);

	//ブロック同士をつなぐ更新関数
	void UpdateConnect(Vec3 pos, bool isConnect);

	//繋ぐ際に離したところが軸以外のボタンかどうか
	bool CheckAxisButton(Vec3 pos, bool isConnect);

	//繋がれているブロックを全部解除する
	void ReleseConectedBlock();

	//キーボードによって回転
	//キーボードを関数内で取得
	void UpdateRotatePlayerBlock(Vec3& rotatePos, bool isRota);

public:

	static const int blockWidth = 13;
	static const int blockHeight = 13;

private:
	
	Model* model_ = nullptr;
	
	//ブロックの二次元配列
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	//std::vector< std::unique_ptr<std::vector<Block>> > blocks_;

	Block* blocks_[blockWidth][blockHeight];

	//ワールド変換データ
	WorldMat worldTransform_[blockWidth][blockHeight];
	WorldMat preWorldTransform_[blockWidth][blockHeight];

	//状態変化変数
	Form form_[blockWidth][blockHeight] = { Form::BLOCK };

	Action action_[blockWidth][blockHeight] = { Action::None };

	//マップチップで管理するフラグ
	////回転しているかどうか
	//bool isRotate_[blockWidth][blockHeight];
	////重なっているかどうか
	//bool isOverlap_[blockWidth][blockHeight];
	////繋がっているかどうか
	//bool isConnect_[blockWidth][blockHeight];
	//軸になっているかどうか
	bool isAxis_[blockWidth][blockHeight];

	//選択されているかどうか
	int isCount;

	//ブロックの大きさ
	Vec3 scale_;

	//半径
	float radius_;

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

};

