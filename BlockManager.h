#pragma once
#include "Block.h"
#include <vector>

class BlockManager
{

public:

	//初期化
	void Initialize();

	//ステージの関数で先にブロックあるか判定(endPosを引数)
	bool GetPosIsBlock(Vec3 pos);

	//ボタンがあるかどうか
	bool GetPosIsButton(Vec3 pos);

	//最初に繋ぐボタンを押したブロックを軸に登録する関数
	void RegistButton(const Vec3& pos);

	//ブロック同士をつなぐ更新関数
	void UpdateConnect(Vec3 pos);

	//繋ぐ際に離したところが軸以外のボタンかどうか
	bool CheckAxisButton(Vec3 pos);

	//繋がれているブロックを全部解除する
	void ReleseConectedBlock();

	//キーボードによって回転
	//キーボードを関数内で取得
	void UpdateRotatePlayerBlock(Vec3& rotatePos);


private:
	
	//ブロックの二次元配列
	std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;

};

