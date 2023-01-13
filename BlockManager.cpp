#include "BlockManager.h"

//初期化
void BlockManager::Initialize()
{
	//ブロックを生成し初期化
	blocks_ = std::make_unique<std::vector <std::vector<Block>>>();
}

//ステージの関数で先にブロックあるか判定(endPosを引数)
bool BlockManager::GetPosIsBlock(Vec3 pos)
{
	Vec3 playerPos = pos;

	

	return false;
}

//ボタンがあるかどうか
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	return false;
}

//最初に繋ぐボタンを押したブロックを軸に登録する関数
void BlockManager::RegistButton(const Vec3& pos)
{
	//最初にボタンを押したブロックを軸に登録する関数
	//引数で受け取ったプレイヤーの座標をもとに現在位置のボタンをONにする

	//プレイヤーの位置にあるブロックを軸にする
	

}

//ブロック同士をつなぐ更新関数
void BlockManager::UpdateConnect(Vec3 pos)
{
}

//繋ぐ際に離したところが軸以外のボタンかどうか
bool BlockManager::CheckAxisButton(Vec3 pos)
{


	return false;
}

//繋がれているブロックを全部解除する
void BlockManager::ReleseConectedBlock()
{
}

//キーボードによって回転
//キーボードを関数内で取得
void BlockManager::UpdateRotatePlayerBlock(Vec3& rotatePos)
{
}
