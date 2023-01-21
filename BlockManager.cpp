#include "BlockManager.h"
#include <fstream>
#include "ParticleManager.h"


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
void BlockManager::Initialize(ConnectingEffectManager* connectEM, Tutorial* tutorial, CameraManager* cameraM, GoalEffect* goalEffect,
	Model* normal, Model* button, Model* goal, Model* Socket)
{
	blocks_.clear();
	worldmats_.clear();

	this->connectEM = connectEM;
	this->cameraM = cameraM;
	this->tutorial = tutorial;
	this->goalEffect = goalEffect;
	this->goalCameraPoses.clear();

	//std::unique_ptr<Block> newBullet = std::make_unique<Block>();

	if (texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/effect1.png", texhandle[0]);
	}

	//初期化
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

			//ブロックの座標を設定
			if (i >= 0)
			{
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);
			}
			if (j >= 0)
			{
				worldmats_[i][j].trans.z = j * (worldmats_[i][j].scale.z * 2.0f);
			}

			//worldmats_[i][j].trans.y = worldmats_[i][j].scale.y;

			worldmats_[i][j].SetWorld();

			block_->Initialize(connectEM, normal, button, goal, Socket);

			//軸になっているかどうか
			isAxis_[i][j] = false;

			//現在どうなっているか
			action_[i][j] = Action::None;

			isTurn[i][j] = false;


			beforeTurn_[i][j] = form_[i][j];

			isUp[i][j] = false;

		}
	}

	//その他の設定
	isCount = 1;

	changedAction_ = false;
	isChanged_ = false;
	//回転

	isRightRolling = false;
	isLeftRolling = false;

	rotateCount = 0;

	angle_ = 0;

	effectCount = 0;
}

void BlockManager::Update()
{

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{


			//X座標の一つ前の番号を保存
			prevBlockY = j;

			DownPosY();

			blocks_[i][j]->Updata();
			
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

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//Manager.cppで配列で定義したworldTransformの値をBlock.cppのDrawにセット
			blocks_[i][j]->SetWorldPos(worldmats_[i][j].trans);
			//draw->DrawCube3D(worldmats_[i][j], &camera->viewMat, &camera->projectionMat);
			blocks_[i][j]->Draw(camera, texhandle, form_[i][j], action_[i][j]);


			if (action_[i][j] == Action::Connect && effectCount >= effectCountMax)
			{
				if (isAxis_[i][j])
				{
					connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 15, 3, { 1.0f,0.3f,0.2f,1.0f });
				}
				else
				{
					connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 10, 1, { 1.0f,1.0f,0,1.0 });
				}

				isEffect = true;
			}
		}
	}

	if (isEffect) { effectCount = 0; isEffect = false; }
}

bool BlockManager::CheckPlayerOnBlock(Vec3 pos)
{
	bool result = false;

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
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
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{

			//プレイヤーがブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ <= pos.x && worldmats_[i][j].trans.x + blockRadius_ >= pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ <= pos.z && worldmats_[i][j].trans.z + blockRadius_ >= pos.z)
			{
				//そのブロックの形状は普通のブロックかどうか
				if (form_[i][j] != Form::NONE && form_[i][j] != Form::LOCKED && action_[i][j] != Action::Connect)
				{
					return true;
				}

			}
		}
	}

	//playerがどのブロックにもいない
	return false;
}


//ギアがあるかどうか
bool BlockManager::GetPosIsGear(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状はボタンかどうか
				if (form_[i][j] == Form::GEAR)
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
void BlockManager::RegistAxisGear(const Vec3& pos)
{
	//最初にボタンを押したブロックを軸に登録する関数
	//引数で受け取ったプレイヤーの座標をもとに現在位置のボタンをONにする
	 
	//プレイヤーの位置にあるブロックを軸にする
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状はボタンかどうか
				if (form_[i][j] == Form::GEAR && isAxis_[i][j] == false)
				{
					//軸登録する
					isAxis_[i][j] = true;
					//軸のブロックの座標を得る
					axis_pos_.x = worldmats_[i][j].trans.x;
					axis_pos_.y = worldmats_[i][j].trans.y;
					axis_pos_.z = worldmats_[i][j].trans.z;

					cameraM->usingCamera->CameraShake(15, 1.3f);
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
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if ((worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
				&& action_[i][j] != Action::Connect)
			{
				if (form_[i][j] != Form::NONE)
				{
					action_[i][j] = Action::Connect;
					cameraM->usingCamera->CameraShake(15, 0.53f);

				}
			}
		}
	}
}

//繋ぐ際に離したところが軸以外のギアかどうか
bool BlockManager::CheckAxisGear(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//プレイヤーがいるブロック内において
			if ((worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z))
			{
				if (isAxis_[i][j] == false && form_[i][j] == Form::GEAR)
				{
					cameraM->usingCamera->CameraShake(15, 1.2f);
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
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
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

	if (isLeftRolling == false && isRightRolling == false && (KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_D)))
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

	if (isLeftRolling == false && isRightRolling == false && (KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_A)))
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

		//角度が必要
		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
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
			cameraM->usingCamera->CameraShake(15, 1.5f);

			//チュートリアル
			if (tutorial->GetState() == TUTORIAL::TURN)
			{
				tutorial->AddStateNum();
			}

			//パーティクル発生
			GenerateParticleTurnBlock();
		}

	}

	if (isLeftRolling == true)
	{
		rotateCount++;

		//角度が必要
		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
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
			cameraM->usingCamera->CameraShake(15, 1.5f);

			//チュートリアル
			if (tutorial->GetState() == TUTORIAL::TURN)
			{
				tutorial->AddStateNum();
			}

			//パーティクル発生
			GenerateParticleTurnBlock();
		}
	}

	if (isLeftRolling == false && isRightRolling == false)
	{
		//DownPosY();
		UpdateOverlap();
	}

	else if (isLeftRolling == true || isRightRolling == true)
	{
		UpPosY();
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

bool BlockManager::GetIsGoal(Vec3& pos, bool isPlayer)
{

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状はゴールかどうか
				if (form_[i][j] == Form::GOAL)
				{
					//�S�[�����o
					if (isPlayer)
					{
						Vec3 goalEyeDistance = worldmats_[i][j].trans - cameraM->gameMainCamera->GetEye();

						for (int i = 0; i < 4; i++)
						{
							if (i < 2) {
								goalCameraPoses.push_back(cameraM->gameMainCamera->GetEye() + Vec3{ goalEyeDistance.x,goalEyeDistance.y + i * 60.0f ,goalEyeDistance.z } / 4.0f * i);
							}
							else if (i == 4 - 2)
							{
								goalCameraPoses.push_back(cameraM->gameMainCamera->GetEye() + Vec3{ goalEyeDistance.x,goalEyeDistance.y - blockRadius_ * 10.0f ,goalEyeDistance.z } / 4.0f * i);
							}
							else
							{
								goalCameraPoses.push_back(cameraM->gameMainCamera->GetEye() + goalEyeDistance / 4.0f * i);
							}
						}

						goalEffect->BegineGoalEffect(goalCameraPoses, { worldmats_[i][j].trans.x, worldmats_[i][j].trans.y + blockRadius_ * 2.0f,worldmats_[i][j].trans.z }, 30);
					}

					//カメラ切り替え
					cameraM->usingCamera = cameraM->goalEffectCamera.get();

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
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			for (int k = 0; k < stageWidth_; k++)
			{
				for (int l = 0; l < stageHeight_; l++)
				{
					//重なった時に形を変える処理
					if (CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans) && form_[k][l] != Form::NONE && form_[i][j] != Form::NONE)
					{
						//同じ座標ではないとき
						if (i != k || j != l)
						{
							/*beforeTurn_[i][j] = form_[i][j];
							beforeTurn_[k][l] = form_[k][l];*/

							if (form_[i][j] != Form::NONE && form_[k][l] != Form::NONE)
								//if (form_[i][j] != Form::GOAL && form_[k][l] != Form::GOAL)
							{
								//if(action_[i][j] == Action::Connect || action_[k][l] == Action::Connect)
								//重なっているブロック両方を固定ブロック化


								//回転させる前の状態を保存
								if (isTurn[i][j] == false || isTurn[k][l] == false)
								{
									beforeTurn_[i][j] = form_[i][j];
									beforeTurn_[k][l] = form_[k][l];

									isOver[i][j] = true;
									isOver[k][l] = true;

									//���o
									form_[i][j] = Form::LOCKED;
									form_[k][l] = Form::LOCKED;
									
									if(isUp[i][j] == true)
									{
										//ブロックの演出
										blocks_[i][j]->GetWorldTransForm()->scale =
										{ blocks_[i][j]->GetRadius() * 1.8f,blocks_[i][j]->GetRadius() * 1.8f, blocks_[i][j]->GetRadius() * 1.8f };
									}
									else if(isUp[k][l] == true)
									{
										//ブロックの演出
										blocks_[k][l]->GetWorldTransForm()->scale =
										{ blocks_[k][l]->GetRadius() * 1.8f,blocks_[k][l]->GetRadius() * 1.8f, blocks_[k][l]->GetRadius() * 1.8f };
									}
									
								}

								//重なったところを固定状態に
								form_[i][j] = Form::LOCKED;
								form_[k][l] = Form::LOCKED;
								//回転したフラグをONに
								isTurn[i][j] = true;
								isTurn[k][l] = true;
							}


						}

						//チュートリアル
						if (tutorial->GetState() == TUTORIAL::OVERLAP)
						{
							//tutorial->AddStateNum();
						}
					}
				}
			}

			/*if(isUp[i][j] == true)
			{
				worldmats_[i][j].trans.y = beforeTransY[i][j];
				isUp[i][j] = false;
			}*/

		}
	}


}

//重なっていたブロックを元に戻す処理
void BlockManager::RepositBlock()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			for (int k = 0; k < stageWidth_; k++)
			{
				for (int l = 0; l < stageHeight_; l++)
				{
					bool isOverlap = CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans);

					//重なりが外れて元の状態に戻す処理

					//コネクトしているブロックを戻す処理
					if (action_[i][j] == Action::Connect)
					{
						if (isOverlap == true)

							/*if (form_[i][j] == Form::LOCKED && form_[k][l] == Form::LOCKED &&
								action_[i][j] == Action::Connect )*/
						{
							//重なっていたブロックを
							if (form_[i][j] == Form::LOCKED)
							{
								if (i != k || j != l)
								{
									//回転する前の状態に戻す
									form_[i][j] = beforeTurn_[i][j];
									form_[k][l] = beforeTurn_[k][l];

									//回転したフラグをOFFに
									isTurn[i][j] = false;
									isTurn[k][l] = false;
								}
							}
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
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//ブロックの座標を設定
			form_[i][j] = loadForms_[i][j];
			worldmats_[i][j] = loadWorldmats_[i][j];

			worldmats_[i][j].SetWorld();

			//軸になっているかどうか
			isAxis_[i][j] = false;

			//現在どうなっているか
			action_[i][j] = Action::None;
			//回転させる前の状態に、読み込んだ状態を入れる
			beforeTurn_[i][j] = form_[i][j];
			//Y座標を浮かせるフラグを初期化
			isUp[i][j] = false;

		}
	}

	changedAction_ = false;
	isChanged_ = false;
	//回転

	isRightRolling = false;
	isLeftRolling = false;

	rotateCount = 0;

	angle_ = 0;

	effectCount = 0;
}

void BlockManager::GenerateParticleTurnBlock()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (action_[i][j] == Action::Connect)
			{
				//パーティクル発生
				ParticleManager::GetInstance()->GenerateRandomParticle(4, 100, 1.2f, worldmats_[i][j].trans, 0.4f, 0,
					{ 1.0f,1.0f,0.0f,1.0f }, { 1.0f,0.0f,0.0f,1.0f });
			}
		}
	}
}

void BlockManager::UpPosY()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (action_[i][j] == Action::Connect)
			{
				if (form_[i][j] == Form::BLOCK || form_[i][j] == Form::GEAR)
				{
					if(isUp[i][j] == false)
					{
						/*blocks_[i][j]->GetWorldTransForm()->trans =
						{ blocks_[i][j]->GetRadius() * 1.1f,blocks_[i][j]->GetRadius() * 1.1f, blocks_[i][j]->GetRadius() * 1.1f };*/

						beforeTransY[i][j] = worldmats_[i][j].trans.y;
						worldmats_[i][j].trans.y -= 0.40f;
							
						isUp[i][j] = true;
					}
				}
			}
		}
	}
}
void BlockManager::DownPosY()
{
	for(int i = 0; i < stageWidth_; i++)
	{
		for(int j = 0; j < stageHeight_; j++)
		{
			if(action_[i][j] != Action::Connect)
			{
				if(form_[i][j] == Form::BLOCK || form_[i][j] == Form::GEAR)
				{
					if(isUp[i][j] == true)
					{
						
						beforeTransY[i][j] = worldmats_[i][j].trans.y;
						worldmats_[i][j].trans.y += 0.40f;

						isUp[i][j] = false;
					}
				}
			}
		}
	}
}

//読み込んだステージをセットする関数
void BlockManager::SetStage(const int& stageWidth, const int& stageHeight, std::vector<std::vector<WorldMat>>& worldmats, std::vector<std::vector<Form>>& forms)
{
	stageWidth_ = stageWidth;
	stageHeight_ = stageHeight;

	//読み込み用ワールド行列を設定
	for (int i = 0; i < stageWidth_; i++)
	{
		//ワールド行列型を持てる空のベクタを追加(行列でいうi列)
		loadWorldmats_.push_back(vector<WorldMat>());

		for (int j = 0; j < stageHeight_; j++)
		{
			//ワールド行列の要素を追加
			loadWorldmats_[i].push_back(worldmat_);
		}
	}

	//読み込み用の形状を設定
	for (int i = 0; i < stageWidth_; i++)
	{
		//形状の型を持てる空のベクタを追加(行列でいうi列)
		loadForms_.push_back(vector<Form>());

		for (int j = 0; j < stageHeight_; j++)
		{
			//形状の要素を追加
			loadForms_[i].push_back(loadForm_);
		}
	}

	for (int i = 0; i < stageWidth; i++)
	{
		for (int j = 0; j < stageHeight; j++)
		{
			worldmats_[i][j].trans = worldmats[i][j].trans;
			loadWorldmats_[i][j].trans = worldmats[i][j].trans;
			form_[i][j] = forms[i][j];
			loadForms_[i][j] = forms[i][j];
		}
	}
}

void BlockManager::LoadBlockPosData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources\\blockPos.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	blocksPos << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void BlockManager::BlockPop(Vec3 pos)
{
	//敵の生成

	//ベクタ配列に要素<ブロック>を追加
	for (int i = 0; i < stageWidth_; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		blocks_.push_back(vector<Block*>());

		for (int j = 0; j < stageHeight_; j++)
		{
			block_ = new Block;
			//ブロックの要素を追加
			blocks_[i].push_back(block_);
		}
	}

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			worldmats_[i][j].trans = pos;
		}
	}
}

void BlockManager::UpdateBlockPos()
{
	//待機処理
	if (isWaitBlock)
	{
		blockWaitTimer--;
		if (blockWaitTimer <= 0)
		{
			//待機完了
			isWaitBlock = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(blocksPos, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			//コメント行を飛ばす
			continue;
		}

		//POPコマンドcsv
		if (word.find("POP") == 0)
		{
			//X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//ブロックを発生させる
			BlockPop(Vec3(x, y, z));
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWaitBlock = true;
			blockWaitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}

}

