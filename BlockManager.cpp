#include "BlockManager.h"

using namespace std;

BlockManager::~BlockManager()
{

	//ブロックの削除
	blocks_.clear();
	worldmats_.clear();
	delete block_;
	//delete worldmat_;
}

//初期化
void BlockManager::Initialize(ConnectingEffectManager* connectEM, Camera* camera,
	Model* normal, Model* button, Model* goal, Model* Socket)
{
	blocks_.clear();
	worldmats_.clear();

	this->connectEM = connectEM;
	this->camera = camera;

	//std::unique_ptr<Block> newBullet = std::make_unique<Block>();

	if (texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/effect1.png", texhandle[0]);
	}

	//ベクタ配列に要素<ブロック>を追加
	for (int i = 0; i < blockWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		blocks_.push_back(vector<Block*>());

		for (int j = 0; j < blockHeight; j++)
		{
			block_ = new Block;
			//ブロックの要素を追加
			blocks_[i].push_back(block_);
		}
	}

	//ベクタ配列に要素<ワールド行列>を追加
	for (int i = 0; i < blockWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		worldmats_.push_back(vector<WorldMat>());

		for (int j = 0; j < blockHeight; j++)
		{
			//ブロックの要素を追加
			worldmats_[i].push_back(worldmat_);
		}
	}

	//ブロックの大きさ

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

			worldmats_[i][j].scale = { blockRadius_,blockRadius_,blockRadius_ };

			//worldmats_[i][j]->rot = { 0.0f,0.0f,0.0f };

			blocks_[i][j]->Initialize(connectEM, normal, button, goal, Socket);

			//ブロックの種類を設定
			if (i == j)
			{
				form_[i][j] = Form::BUTTON;
			}
			else if (i == 10 && j == 5)
			{
				form_[i][j] = Form::GOAL;
			}
			else
			{
				form_[i][j] = Form::BLOCK;
			}


			//ブロックの座標を設定
			if (i >= 0)
			{
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);
			}
			if (j >= 0)
			{
				worldmats_[i][j].trans.z = j * (worldmats_[i][j].scale.y * 2.0f);
			}

			worldmats_[i][j].SetWorld();

			block_->Initialize(connectEM, normal, button, goal, Socket);

			//軸になっているかどうか
			isAxis_[i][j] = false;

			//現在どうなっているか
			action_[i][j] = Action::None;
		}
	}

	//その他の設定
	isCount = 1;
}

void BlockManager::Update()
{
	if (isLeftRolling == false && isRightRolling == false)
	{
		//UpdateOverlap();
	}
	

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			ResetBlock();

			//X座標の一つ前の番号を保存
			prevBlockY = j;

		}
		//Y座標の一つ前のブロック番号を保存
		prevBlockX = i;
	}

	//状態を変える時の遅延
	if (isChanged_ == false)
	{
		if (--selectTimer_ <= 0)
		{
			isChanged_ = true;
			selectTimer_ = kSelectTime;
		}
	}
}

void BlockManager::Draw(Camera* camera)
{
	bool isEffect = false;
	effectCount++;

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//Manager.cppで配列で定義したworldTransformの値をBlock.cppのDrawにセット
			blocks_[i][j]->SetWorldPos(worldmats_[i][j].trans);
			//draw->DrawCube3D(worldmats_[i][j], &camera->viewMat, &camera->projectionMat);
			blocks_[i][j]->Draw(camera, texhandle, form_[i][j], action_[i][j]);


			//if (action_[i][j] == Action::Connect && effectCount >= effectCountMax)
			//{
			//	if (isAxis_[i][j])
			//	{
			//		connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 35, 5, { 1.0f,0.3f,0.2f,1.0f });
			//	}
			//	else
			//	{
			//		connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 20, 3, { 1.0f,1.0f,0,1.0 });
			//	}

			//	isEffect = true;
			//}
		}
	}

	if (isEffect) { effectCount = 0; isEffect = false; }
}

bool BlockManager::CheckPlayerOnBlock(Vec3 pos)
{
	bool result;

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーがブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}

	return result;
}

//ステージの関数で先にブロックあるか判定(endPosを引数)
bool BlockManager::GetPosIsBlock(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

			//プレイヤーがブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ <= pos.x && worldmats_[i][j].trans.x + blockRadius_ >= pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ <= pos.z && worldmats_[i][j].trans.z + blockRadius_ >= pos.z)
			{
				//そのブロックの形状は普通のブロックかどうか
				if (form_[i][j] != Form::NONE && form_[i][j] != Form::LOCKED)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

	//playerがどのブロックにもいない
	return false;
}

//ボタンがあるかどうか
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状はボタンかどうか
				if (form_[i][j] == Form::BUTTON)
				{
					return true;
				}
			}
		}
	}

	//プレイヤーがどのブロックにもいない場合
	return false;

}

//最初に繋ぐボタンを押したブロックを軸に登録する関数
void BlockManager::RegistAxisButton(const Vec3& pos)
{
	//最初にボタンを押したブロックを軸に登録する関数
	//引数で受け取ったプレイヤーの座標をもとに現在位置のボタンをONにする

	//プレイヤーの位置にあるブロックを軸にする
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状はボタンかどうか
				if (form_[i][j] == Form::BUTTON && isAxis_[i][j] == false)
				{
					//軸登録する
 					isAxis_[i][j] = true;
					//軸のブロックの座標を得る
					axis_pos_.x = worldmats_[i][j].trans.x;
					axis_pos_.y = worldmats_[i][j].trans.y;
					axis_pos_.z = worldmats_[i][j].trans.z;

					camera->CameraShake(10, 1.2f);
				}
				else {}
			}
			//プレイヤーがどのブロックにもいない場合
			else {}
		}
	}

}

//ブロック同士をつなぐ更新関数
void BlockManager::UpdateConnect(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if ((worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
				&& action_[i][j] != Action::Connect)
			{
				if (form_[i][j] != Form::GOAL)
				{
					action_[i][j] = Action::Connect;

				}
				
				camera->CameraShake(10, 0.3f);

				////繋ぎはじめ
				////ボタンかつ繋がっていなければ現在位置をつなぐ状態にする
				//if (form_[i][j] == Form::BUTTON && action_[i][j] == Action::None)
				//{
				//	action_[i][j] = Action::Connect;
				//}

				////繋ぐ処理(右だけテスト)
				//if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHT) && i < blockWidth - 1)
				//{
				//	//前ブロックが繋がっていれば繋げる
				//	if (action_[i][j] == Action::Connect)
				//	{
				//		if (form_[i + 1][j] == Form::BLOCK)
				//		{
				//			//ブロックなら繋げる
				//			action_[i + 1][j] = Action::Connect;
				//			isChanged_ = false;
				//		}
				//		else if (form_[i + 1][j] == Form::GEAR)
				//		{
				//			//ギアなら繋げて止める
				//			action_[i + 1][j] = Action::Connect;
				//			isChanged_ = false;
				//			changedAction_ = false;
				//		}
				//	}
				//}
			}

		}
	}
}

//繋ぐ際に離したところが軸以外のボタンかどうか
bool BlockManager::CheckAxisButton(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーがいるブロック内において
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				if (isAxis_[i][j] == false && form_[i][j] == Form::BUTTON)
				{
					camera->CameraShake(10, 0.7f);
					return true;
				}
			}
		}
	}

	//現在のブロックが軸登録されているならfalse
	return false;
}

//繋がれているブロックを全部解除する
void BlockManager::ReleseConectedBlock()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			if (action_[i][j] == Action::Connect)
			{
				//全部何もしていない状態に
				action_[i][j] = Action::None;
				isAxis_[i][j] = false;
			}
		}
	}
}

//キーボードによって回転

void BlockManager::UpdateRotate(Vec3& rotatePos)
{

	if (isLeftRolling == false && isRightRolling == false && KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW))
	{
		isRightRolling = true;
		rotateCount = 0;
		angle_ = 0;

		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
			}
		}

		distancePosPlayer = rotatePos - axis_pos_;
	}

	if (isLeftRolling == false && isRightRolling == false && KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW))
	{
		isLeftRolling = true;
		rotateCount = 0;
		angle_ = 0;

		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
			}
		}

		distancePosPlayer = rotatePos - axis_pos_;
	}

	if (isRightRolling == true)
	{
		rotateCount++;

		//角度が必要(前にやった円運動が参考になるかも)
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{

				//もしつながっているなら
				if (action_[i][j] == Action::Connect && isAxis_[i][j] == false)
				{
					WorldMat worldMat;
					worldMat.rot.y = LerpVec3({ angle_, 0, 0 }, { pi / 2.0f,0,0 },
						EaseOut((float)rotateCount / (float)rotateCountMax)).x;
					worldMat.SetWorld();

					//ブロックの回転
					worldmats_[i][j].trans.x = axis_pos_.x + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).x;
					worldmats_[i][j].trans.z = axis_pos_.z + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).z;
					worldmats_[i][j].rot.y = worldMat.rot.y;
					worldmats_[i][j].SetWorld();

					//プレイヤーの回転
					rotatePos.x = axis_pos_.x + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).x;

					rotatePos.z = axis_pos_.z + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).z;
				}

			}
		}


		if (rotateCount >= rotateCountMax)
		{
			isRightRolling = false;
			camera->CameraShake(10, 1.4f);
		}

	}

	if (isLeftRolling == true)
	{
		rotateCount++;

		//角度が必要(前にやった円運動が参考になるかも)
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//もしつながっているなら
				if (action_[i][j] == Action::Connect && isAxis_[i][j] == false)
				{
					WorldMat worldMat;
					worldMat.rot.y = LerpVec3({ angle_, 0, 0 }, { -pi / 2.0f,0,0 },
						EaseOut((float)rotateCount / (float)rotateCountMax)).x;
					worldMat.SetWorld();

					//ブロックの回転
					worldmats_[i][j].trans.x = axis_pos_.x + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).x;
					worldmats_[i][j].trans.z = axis_pos_.z + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).z;
					worldmats_[i][j].rot.y = worldMat.rot.y;

					//プレイヤーの回転
					rotatePos.x = axis_pos_.x + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).x;

					rotatePos.z = axis_pos_.z + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).z;

				}
			}
		}


		if (rotateCount >= rotateCountMax)
		{
			isLeftRolling = false;
			camera->CameraShake(10, 1.4f);
		}
	}

	if (isLeftRolling == false && isRightRolling == false)
	{
		UpdateOverlap();
	}
	else if (isLeftRolling == false || isRightRolling == false)
	{
		RepositBlock();
	}
}

bool BlockManager::GetIsRollingLeftorRight()
{
	if (isLeftRolling || isRightRolling)
	{
		return true;
	}
	return false;
}

bool BlockManager::GetIsGoal(Vec3& pos)
{

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状はボタンかどうか
				if (form_[i][j] == Form::GOAL)
				{
					return true;
				}
			}
		}
	}

	//プレイヤーがどのブロックにもいない場合
	return false;
}

//重なった時の処理
void BlockManager::UpdateOverlap()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			for (int k = 0; k < blockWidth; k++)
			{
				for (int l = 0; l < blockWidth; l++)
				{
				
					//重なった時に形を変える処理
					if (CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans))
					{
						//同じ座標ではないとき
						if (i != k || j != l )
						{
							if (form_[i][j] != Form::BUTTON && form_[k][l] != Form::BUTTON)
							{
								//重なっているブロック両方を固定ブロック化
								form_[i][j] = Form::LOCKED;
								form_[k][l] = Form::LOCKED;
							}
						}
					}
					/*else if (CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans) == false)
					{
						if (form_[i][j] == Form::LOCKED && form_[k][l] == Form::LOCKED)
						{
							if (i != k || j != l)
							{
								form_[i][j] = Form::BLOCK;
								form_[k][l] = Form::BLOCK;
							}
						}
					}*/

				}
			}
		}
	}
}

//重なっていたブロックを元に戻す処理
void BlockManager::RepositBlock()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			for (int k = 0; k < blockWidth; k++)
			{
				for (int l = 0; l < blockWidth; l++)
				{
					bool isOverlap = CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans);

					//重なりが外れて元の状態に戻す処理
					if (isOverlap == false && form_[i][j] == Form::LOCKED && form_[k][l] == Form::LOCKED)
					{
						if (i != k || j != l)
						{
							form_[i][j] = Form::BLOCK;
							form_[k][l] = Form::BLOCK;
						}
					}

				}
			}
		}
	}

}

//ブロックブロックの矩形の当たり判定
bool BlockManager::CollisionBlockToBlock(Vec3 blockPos, Vec3 comPos)
{
	if (blockPos.x - blockRadius_ < comPos.x && blockPos.x + blockRadius_ > comPos.x
		&& blockPos.z - blockRadius_ <comPos.z && blockPos.z + blockRadius_ > comPos.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BlockManager::ResetBlock()
{
	if (KeyboardInput::GetInstance().KeyPush(DIK_R))
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//ブロックの座標を設定
				if (i >= 0)
				{
					worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);
				}
				if (j >= 0)
				{
					worldmats_[i][j].trans.z = j * (worldmats_[i][j].scale.y * 2.0f);
				}
			}
		}
	}

}


//
////進む先に重なっているブロックがあるかどうか
//bool BlockManager::GetIsOverlapBlock(Vec3 pos)
//{
//	for (int i = 0; i < blockWidth; i++)
//	{
//		for (int j = 0; j < blockHeight; j++)
//		{
//			//プレイヤーが指定のブロックの上にいるかどうか
//			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
//				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
//			{
//				//そのブロックの形状は重なっているかどうか
//				if (form_[i][j] != Form::LOCKED)
//				{
//					return true;
//				}
//				else
//				{
//					return false;
//				}
//			}
//		}
//	}
//
//	//プレイヤーがどのブロックにもいない場合
//	return false;
//}
