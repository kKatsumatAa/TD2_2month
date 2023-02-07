#include "BlockManager.h"
//#include <fstream>
#include <complex>
#include "ParticleManager.h"
#include "GetBackManager.h"

using namespace std;

BlockManager& BlockManager::operator=(const BlockManager& obj)
{
	/*this->Initialize(obj.connectEM, obj.predictBlockM, obj.tutorial, obj.cameraM, obj.goalEffect, obj.normal, obj.locked, obj.goal, obj.Socket, obj.button, obj.disconnectedBlock,
		obj.disconnectedButton, obj.disconnectedSocketBlock, obj.electricBlock, obj.doorGoalClosed);*/

		//ポインタは削除される可能性があるので中身のみコピー
	this->cameraM = obj.cameraM;
	this->tutorial = obj.tutorial;
	this->goalEffect = obj.goalEffect;
	this->model_ = obj.model_;
	this->goalCameraPoses = obj.goalCameraPoses;
	this->block_ = obj.block_;
	this->blocks_ = obj.blocks_;
	this->worldmat_ = obj.worldmat_;
	this->worldmats_ = obj.worldmats_;
	this->axis_pos_ = obj.axis_pos_;
	this->isCount = obj.isCount;
	this->scale_ = obj.scale_;
	//this->prevBlockX = obj.prevBlockX;
	//this->prevBlockY = obj.prevBlockY;
	//this->selectTimer_ = obj.selectTimer_;
	//this->changedAction_ = obj.changedAction_;
	//this->isChanged_ = obj.isChanged_;
	this->isRightRolling = obj.isRightRolling;
	this->isLeftRolling = obj.isLeftRolling;
	this->rotateCount = obj.rotateCount;
	this->angle_ = obj.angle_;
	this->distancePosPlayer = obj.distancePosPlayer;
	this->connectEM = obj.connectEM;
	this->effectCount = obj.effectCount;
	this->effectCount2 = obj.effectCount2;

	this->isWaitBlock = obj.isWaitBlock;
	this->blockWaitTimer = obj.blockWaitTimer;
	this->loadForm_ = obj.loadForm_;
	this->loadForms_ = obj.loadForms_;
	this->loadWorldmats_ = obj.loadWorldmats_;
	this->stageWidth_ = obj.stageWidth_;
	this->stageHeight_ = obj.stageHeight_;
	this->pushedCount_ = obj.pushedCount_;
	this->needGoalCount = obj.needGoalCount;
	this->isPopGoal = obj.isPopGoal;
	this->goalPos = obj.goalPos;
	this->goalMat = obj.goalMat;
	//isPopedGoal はやんなくて良い
	this->isPopGoalEffect = obj.isPopGoalEffect;
	this->goalPopX = obj.goalPopX;
	this->goalPopY = obj.goalPopY;
	this->isCheckElec_ = obj.isCheckElec_;
	this->checkCount = obj.checkCount;
	if (predictBlockM == nullptr)
	{
		this->predictBlockM = new PredictBlockManager();
	}
	*this->predictBlockM = *obj.predictBlockM;
	this->isConectedGoal = obj.isConectedGoal;
	this->isChangedConectGoal = obj.isChangedConectGoal;
	this->isElecConectedGoal = obj.isElecConectedGoal;
	this->isStopElecConectedGoal = obj.isStopElecConectedGoal;
	this->isStartPop = obj.isStartPop;
	this->isPopWait = obj.isPopWait;
	if (rockOnImage == nullptr)
	{
		this->rockOnImage = new RockOnImage();
	}
	*this->rockOnImage = *obj.rockOnImage;

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			this->isGoal_[i][j] = obj.isGoal_[i][j];
			this->beforeTransY[i][j] = obj.beforeTransY[i][j];
			this->isPushed[i][j] = obj.isPushed[i][j];
			this->isOverLap_[i][j] = obj.isOverLap_[i][j];
			this->beforeTurn_[i][j] = obj.beforeTurn_[i][j];
			this->isUp[i][j] = obj.isUp[i][j];
			this->isDown[i][j] = obj.isDown[i][j];
			this->isTurn[i][j] = obj.isTurn[i][j];
			this->distancePos[i][j] = obj.distancePos[i][j];
			this->comparisonPos[i][j] = obj.comparisonPos[i][j];
			this->transforms[i][j] = obj.transforms[i][j];
			this->beforeForm_[i][j] = obj.beforeForm_[i][j];
			this->action_[i][j] = obj.action_[i][j];
			this->isAxis_[i][j] = obj.isAxis_[i][j];
			this->form_[i][j] = obj.form_[i][j];
			this->formTmp_[i][j] = obj.formTmp_[i][j];
			this->isElec[i][j] = obj.isElec[i][j];
			this->isDecisionElec[i][j] = obj.isDecisionElec[i][j];
			this->isTurning[i][j] = obj.isTurning[i][j];
		}
	}

	return *this;
}

BlockManager::~BlockManager()
{

	//ブロックの削除
	blocks_.clear();
	worldmats_.clear();
	delete block_;
	//delete worldmat_;
}

//初期化
void BlockManager::Initialize(RockOnImage* rockOnImage,ConnectingEffectManager* connectEM, PredictBlockManager* pBM, Tutorial* tutorial, CameraManager* cameraM, GoalEffect* goalEffect,
	GoalConnectEffectManager* goalConnectEM,
	Model* normal, Model* locked, Model* goal, Model* Socket, Model* button, Model* disconnectedBlock,
	Model* disconnectedButton, Model* disconnectedSocketBlock, Model* electricBlock, Model* doorGoalClosed, Model* overLapBlock, Model* beforeButtonPop)
{
	blocks_.clear();
	worldmats_.clear();

	this->connectEM = connectEM;
	this->cameraM = cameraM;
	this->tutorial = tutorial;
	this->goalEffect = goalEffect;
	this->goalConnectEM = goalConnectEM;
	this->predictBlockM = pBM;
	this->goalCameraPoses.clear();

	this->normal = normal; this->locked = locked; this->goal = goal; this->Socket = Socket; this->button = button; this->disconnectedBlock = disconnectedBlock;
	this->disconnectedButton = disconnectedButton; this->disconnectedSocketBlock = disconnectedSocketBlock; this->electricBlock = electricBlock; this->doorGoalClosed = doorGoalClosed;

	this->rockOnImage = rockOnImage;

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

			blocks_[i][j]->Initialize(connectEM, normal, locked, goal, Socket, button, disconnectedBlock,
				disconnectedButton, disconnectedSocketBlock, electricBlock, doorGoalClosed, overLapBlock, beforeButtonPop);

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

			block_->Initialize(connectEM, normal, locked, goal, Socket, button, disconnectedBlock,
				disconnectedButton, disconnectedSocketBlock, electricBlock, doorGoalClosed, overLapBlock, beforeButtonPop);

			//軸になっているかどうか
			isAxis_[i][j] = false;

			//現在どうなっているか
			action_[i][j] = Action::None;

			isTurn[i][j] = false;


			beforeTurn_[i][j] = form_[i][j];

			isUp[i][j] = false;

			isPushed[i][j] = false;

			pushedCount_ = 0;

			isTurning[i][j] = false;
		}

		InitializeElectric();
	}

	isConectedGoal = false;
	isChangedConectGoal = false;

	//繋がっているフラグ初期化
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			for (int k = 0; k < stageWidth_; k++)
			{
				for (int l = 0; l < stageHeight_; l++)
				{
					//同じ番号を防ぐ
					if (i == k && j == l) {}
					else
					{
						if (form_[i][j] == Form::GOAL)
						{
							//もし[i][j]がゴールの時に[k][l]番のブロックと隣接していたら
							if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == true)
							{
								isConectedGoal = true;
								isChangedConectGoal = true;
							}
						}
					}

				}
			}
		}
	}

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//もしゴールに電気が通っていて道が繋がっているなら
			if (isElec[i][j] == true && form_[i][j] == Form::GOAL && isConectedGoal == true)
			{
				isStopElecConectedGoal = true;
				//ゴールできるフラグON
				isElecConectedGoal = true;
			}
			else if (isStopElecConectedGoal == false)
			{
				//ゴールできるフラグON
				isElecConectedGoal = false;
			}
		}
	}

	//for文のストッパーをリセットする
	isStopElecConectedGoal = false;


	//その他の設定
	isCount = 1;

	//回転

	isRightRolling = false;
	isLeftRolling = false;

	rotateCount = 0;

	angle_ = 0;

	effectCount = 0;
	effectCount2 = 0;

	isPopedGoal = false;
	isPopedGoal2 = false;
	isPopedGoal2Count = 0;

	isStartPop = true;

	this->isPopGoalEffect = false;
	//this->goalPopX = 0;
	//this->goalPopY = 0;
}

void BlockManager::Update()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//ゴールの出現
			AppearGoal();
			//演出用にY座標を下げる
			DownPosY();

			blocks_[i][j]->Updata({ 0,0,0 }, form_[i][j], action_[i][j], isElec[i][j], count, goalMat, isGoal_[i][j]);
			blocks_[i][j]->SetAlpha(elecWaitAlpha_[i][j]);


			if (action_[i][j] == Action::Connect)
			{
				if (isLeftRolling == true && isRightRolling == true)
				{
					isElec[i][j] = false;
				}
			}

			//もしゴールに電気が通っていて道が繋がっているなら
			if (isElec[i][j] == true && form_[i][j] == Form::GOAL && isConectedGoal == true && isElecConectedGoal == false)
			{
				isStopElecConectedGoal = true;
				//ゴールできるフラグON
				isElecConectedGoal = true;


			}
			else if (isStopElecConectedGoal == false)
			{
				//ゴールできるフラグON
				isElecConectedGoal = false;
			}

			//回転時のみにしか判定を行ってなかったのでここに移動
			for (int k = 0; k < stageWidth_; k++)
			{
				for (int l = 0; l < stageHeight_; l++)
				{
					if (form_[i][j] == Form::GOAL)
					{
						//同じ座標ではないとき
						if (i != k || j != l)
						{
							if (form_[k][l] != Form::NONE && form_[k][l] != Form::LOCKED)
							{
								//もしゴールがブロックと隣接していたら
								if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == true)
								{
									isConectedGoal = true;
								}
							}
						}
					}
				}
			}
		}
	}

	if (isLeftRolling == false && isRightRolling == false)
	{
		if (isStartPop == true || isPopWait == true)
		{
			PopEffect();
		}
	}

	//for文のストッパーをリセットする
	isStopElecConectedGoal = false;

	goalMat.scale = { Vec3(blockRadius_,blockRadius_,blockRadius_) };

	//状態を変える時の遅延
	if (isChanged_ == false)
	{
		if (--selectTimer_ <= 0)
		{
			isChanged_ = true;
			selectTimer_ = kSelectTime;
		}
	}

	//カメラ演出の後にゴール出現
	if (cameraM != nullptr && (cameraM->isLerpEnd && cameraM->isLerpMoving && isPopGoal && isPopGoalEffect))
	{
		isPopedGoal = true;
		isPopGoalEffect = false;
		isPopGoal = false;
		form_[goalPopX][goalPopY] = Form::GOAL;
		blocks_[goalPopX][goalPopY]->SetScale({ 0,0,0 });

		isPopedGoal2Count = isPopedGoalCountTmp;

		//エフェクト
		ParticleManager::GetInstance()->GenerateRandomParticle(50, 120, 0.5f,
			{ worldmats_[goalPopX][goalPopY].trans.x,worldmats_[goalPopX][goalPopY].trans.y + blockRadius_ * 2.0f, worldmats_[goalPopX][goalPopY].trans.z },
			0.4f, 0, { 1.0f,0.3f,0.2f,1.0f }, { 1.0f,1.0f,0,0 });

		//音
		Sound::GetInstance().PlayWave("emergeGoal.wav", 0.8f);
	}
	//ゴール出現後待つための
	if (isPopedGoal && isPopedGoal2)
	{
		if (isPopedGoal2Count <= 0)
		{
			isPopedGoal2 = false;
		}

		isPopedGoal2Count--;
	}

	//ゴールがほかのブロックとつながってて電気通っていたら
	if (isElecConectedGoal && count % 4 == 0)
	{
		//Object::effectFlags.isEmboss = true;

		for (int i = 0; i < stageHeight_; i++)
		{
			for (int j = 0; j < stageWidth_; j++)
			{
				if (form_[i][j] == Form::GOAL)
				{
					float vec[3] = { ((float)rand() / RAND_MAX - 0.5f) * 0.3f,((float)rand() / RAND_MAX - 0.5f) * 0.3f,((float)rand() / RAND_MAX - 0.5f) * 0.3f };

					int time = (float)rand() / RAND_MAX * 60;

					float scale[2] = { (float)rand() / RAND_MAX * 1.3f,(float)rand() / RAND_MAX };
					float pos = (float)rand() / RAND_MAX * blockRadius_ * 4.0f;
					float pos2 = ((float)rand() / RAND_MAX - 0.5f) * blockRadius_ * 3.0f;

					XMFLOAT4 color = { (float)rand() / RAND_MAX ,(float)rand() / RAND_MAX ,(float)rand() / RAND_MAX ,(float)rand() / RAND_MAX + 0.3f };

					ParticleManager::GetInstance()->Add(time, { worldmats_[i][j].trans.x + pos2,worldmats_[i][j].trans.y + pos ,worldmats_[i][j].trans.z },
						{ vec[0],vec[1],vec[2] }, { -vec[0] / (float)time,-vec[1] / (float)time,-vec[2] / (float)time }, scale[0], scale[1], { 0.1,0.2,1.0,color.w }, { 0,0,0,0 });

					goalConnectEM->GenerateGoalConnectEffect({ worldmats_[i][j].trans.x + pos2,worldmats_[i][j].trans.y + pos ,worldmats_[i][j].trans.z },
						{ scale[0] * 0.3f ,scale[0] * 0.3f,scale[0] * 0.3f }, { 0,0,0 }, { 1.0f,1.0f,0.2f,0.6f }, { 0,0,0,0 }, 60, { vec[0],vec[1],vec[2] });

				}
			}
		}
	}
}

void BlockManager::Draw(Camera* camera)
{
	bool isEffect = false;
	bool isEffect2 = false;

	effectCount++;
	effectCount2++;
	count++;


	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//Manager.cppで配列で定義したworldTransformの値をBlock.cppのDrawにセット
			blocks_[i][j]->SetWorldPos(worldmats_[i][j].trans);
			//draw->DrawCube3D(worldmats_[i][j], &camera->viewMat, &camera->projectionMat);
			blocks_[i][j]->Draw(camera, texhandle, form_[i][j], action_[i][j], isElec[i][j], isPushed[i][j], this->count, elecWaitAlpha_[i][j], isGoal_[i][j], isPopGoal);

			if (action_[i][j] == Action::Connect && effectCount >= effectCountMax)
			{
				if (isAxis_[i][j])
				{
					connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 15, 3, { 1.0f,0.3f,0.2f,0.95f });
				}
				else
				{
					connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 10, 1, { 1.0f,1.0f,0,1.0 });
				}

				isEffect = true;
			}

			if (form_[i][j] == Form::Electric && form_[i][j] != Form::GOAL && !isElecConectedGoal)
			{
				connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 15, 3, { 0.3f,0.3f,1.0f,0.95f });

				isEffect2 = true;
			}
		}
	}

	if (isEffect) { effectCount = 0; isEffect = false; }
	if (isEffect2) { effectCount2 = 0; isEffect2 = false; }
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
				if (form_[i][j] == Form::GOAL && isElec[i][j] == false)
				{
					return false;
				}
				//そのブロックの形状は普通のブロックかどうか
				else if (form_[i][j] != Form::NONE && form_[i][j] != Form::LOCKED && form_[i][j] != Form::Electric && form_[i][j] != Form::BUTTON && action_[i][j] != Action::Connect)
				{
					if (isPushed[i][j] == false)
					{
						return true;
					}
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
				if (form_[i][j] != Form::NONE && form_[i][j] != Form::BUTTON && form_[i][j] != Form::GOAL && beforeTurn_[i][j] != Form::GOAL)
				{
					action_[i][j] = Action::Connect;
					cameraM->usingCamera->CameraShake(15, 0.53f);

					//チュートリアル
					if (tutorial->GetState() == TUTORIAL::CONNECT_LIMIT)
					{
						tutorial->AddStateNum();
						if (tutorial->GetStateNum() == tutorial->GetStateNumMax() - 1)
						{
							tutorial->spriteCount = 0;
						}
					}

					//音
					Sound::GetInstance().PlayWave("connectMove.wav", 1.0f);
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
					//回転用の距離
					for (int i = 0; i < blockWidth; i++)
					{
						for (int j = 0; j < blockHeight; j++)
						{
							distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
						}
					}

					//予測線
					GeneratePredictBlock();

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
	int prerotateX = 0;
	int prerotateY = 0;

	//回転させる前の座標を保存

	if (isLeftRolling == false && isRightRolling == false && (KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_D)))
	{
		//一手戻る機能にセーブする
		GetBackManager::GetInstance()->SaveDatas();

		isRightRolling = true;
		rotateCount = 0;
		angle_ = 0;

		//回転用の距離
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				isTurning[i][j] = false;
				distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
			}
		}

		predictBlockM->ClearPredictBlock();

		distancePosPlayer = rotatePos - axis_pos_;

		//音
		Sound::GetInstance().PlayWave("turnBegine.wav", 0.7f);

	}

	if (isLeftRolling == false && isRightRolling == false && (KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_A)))
	{
		//一手戻る機能にセーブする
		GetBackManager::GetInstance()->SaveDatas();

		isLeftRolling = true;
		rotateCount = 0;
		angle_ = 0;

		//回転用の距離
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
			}
		}

		predictBlockM->ClearPredictBlock();

		distancePosPlayer = rotatePos - axis_pos_;

		//音
		Sound::GetInstance().PlayWave("turnBegine.wav", 0.7f);
	}

	if (isRightRolling == true)
	{
		rotateCount++;
		isCheckElec_ = false;
		checkCount = 0;


		//角度が必要
		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
			{

				//もしつながっているなら
				if (action_[i][j] == Action::Connect && isAxis_[i][j] == false && beforeTurn_[i][j] != Form::GOAL)
				{
					isTurning[i][j] = true;

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

			//回転用の距離
			for (int i = 0; i < blockWidth; i++)
			{
				for (int j = 0; j < blockHeight; j++)
				{
					isTurning[i][j] = false;
					distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
				}
			}

			//予測線
			GeneratePredictBlock();

			//パーティクル発生
			GenerateParticleTurnBlock();

			//音
			Sound::GetInstance().PlayWave("turnEnd.wav", 0.5f);//
		}

	}

	if (isLeftRolling == true)
	{

		rotateCount++;
		isCheckElec_ = false;
		checkCount = 0;

		//角度が必要
		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
			{
				//もしつながっているなら
				if (action_[i][j] == Action::Connect && isAxis_[i][j] == false && beforeTurn_[i][j] != Form::GOAL)
				{
					isTurning[i][j] = true;

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

			//回転用の距離
			for (int i = 0; i < blockWidth; i++)
			{
				for (int j = 0; j < blockHeight; j++)
				{
					isTurning[i][j] = false;
					distancePos[i][j] = worldmats_[i][j].trans - axis_pos_;
				}
			}

			//予測線
			GeneratePredictBlock();

			//パーティクル発生
			GenerateParticleTurnBlock();

			//音
			Sound::GetInstance().PlayWave("turnEnd.wav", 0.5f);//
		}
	}

	if (isLeftRolling == false && isRightRolling == false)
	{
		UpdateOverlap();

		ConectElec();
		//SplineElecConect();
	}
	else if (isLeftRolling == true || isRightRolling == true)
	{
		UpPosY();

		if (rotateCount >= rotateCountMax - 1)
		{

			//一回全部OFFにする
			for (int i = 0; i < stageWidth_; i++)
			{
				for (int j = 0; j < stageHeight_; j++)
				{
					isElec[i][j] = false;
				}
			}
		}

		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
			{
				//電気ブロックの位置のスイッチをONにする
				if (form_[i][j] == Form::Electric)
				{
					isElec[i][j] = true;
				}

				for (int k = 0; k < stageWidth_; k++)
				{
					for (int l = 0; l < stageHeight_; l++)
					{
						//隣接判定
						if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == true)
						{
							//同じ座標ではないとき
							if (i != k || j != l)
							{
								if (isElec[i][j] == true && form_[k][l] != Form::NONE && form_[k][l] != Form::LOCKED && isTurning[k][l] == false)
								{
									isElec[k][l] = true;
									//isDecisionElec[k][l] = true;
								}
								else if (form_[k][l] == Form::NONE || form_[k][l] == Form::LOCKED || isTurning[k][l] == true)
								{
									isElec[k][l] = false;
									//isDecisionElec[k][l] = false;
								}
								else if (form_[i][j] == Form::BUTTON && isPushed[i][j] == true)
								{
									isElec[i][j] = false;
								}
								else if (form_[k][l] == Form::BUTTON && isPushed[k][l] == true)
								{
									isElec[k][l] = false;
								}
								else if (isTurn[i][j] == true)
								{
									isElec[i][j] = false;
								}
								else if (isTurn[k][l] == true)
								{
									isElec[k][l] = false;
								}

							}
						}

						if (CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans) && form_[k][l] != Form::NONE && form_[i][j] != Form::NONE)
						{
							if (form_[i][j] == Form::LOCKED)
							{
								isElec[i][j] = false;
							}
							if (form_[k][l] == Form::LOCKED)
							{
								isElec[k][l] = false;
							}
						}

						if (action_[i][j] == Action::Connect)
						{
							if (isLeftRolling == true && isRightRolling == true)
							{
								isElec[i][j] = false;
							}
						}
					}
				}

				if (isTurn[i][j] == true)
				{
					isElec[i][j] = false;
				}

				prerotateY = j;
			}

			prerotateX = i;
		}

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
				if (form_[i][j] == Form::GOAL && isElec[i][j] == true)
				{
					if (isPlayer)
					{
						Vec3 goalEyeDistance = worldmats_[i][j].trans - cameraM->gameMainCamera->GetEye();

						for (int i = 0; i < 4; i++)
						{
							if (i < 2)
							{
								goalCameraPoses.push_back(cameraM->gameMainCamera->GetEye() + Vec3{ goalEyeDistance.x,goalEyeDistance.y + i * 60.0f ,goalEyeDistance.z } / 4.0f * float(i));
							}
							else if (i == 4 - 2)
							{
								goalCameraPoses.push_back(cameraM->gameMainCamera->GetEye() + Vec3{ goalEyeDistance.x,goalEyeDistance.y - blockRadius_ * 10.0f ,goalEyeDistance.z } / 4.0f * float(i));
							}
							else
							{
								goalCameraPoses.push_back(cameraM->gameMainCamera->GetEye() + goalEyeDistance / 4.0f * float(i));
							}
						}

						goalEffect->BegineGoalEffect(goalCameraPoses, { worldmats_[i][j].trans.x, worldmats_[i][j].trans.y + blockRadius_ * 2.0f,worldmats_[i][j].trans.z }, 30,
							3, 20);
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

bool BlockManager::GetisLockedBlock(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//そのブロックの形状は重なっているブロックかどうか
				if (form_[i][j] == Form::LOCKED)
				{
					return true;
				}
			}
		}
	}

	//プレイヤーが重なっているブロックにいる場合
	return false;
}

void BlockManager::PopEffect()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			elecCount_[i][j]++;

			//timeRateが1.0f以上になったら、次の区間に進む
			timeRate[i][j] = elecCount_[i][j] / maxTime;

			if (isStartPop == true && isPopWait == false)
			{
				Vec3 start{ -500.0f, 0.0f, 0.0f };
				Vec3 p2{ 80.0f, 0.0f, 0.0f };
				Vec3 p3{ 100.0f, 20.0f, 0.0f };
				/*Vec3 p4{ 100.0f, 160.0f, 0.0f };*/
				Vec3 end{ 100.0f, 250.0f, 0.0f };

				Vec3 a = LerpVec3(start, p2, timeRate[i][j]);
				Vec3 b = LerpVec3(p2, p3, timeRate[i][j]);
				Vec3 c = LerpVec3(p3, end, timeRate[i][j]);

				Vec3 d = LerpVec3(a, b, timeRate[i][j]);
				Vec3 e = LerpVec3(b, c, timeRate[i][j]);

				timerMoveResult_[i][j] = LerpVec3(d, e, timeRate[i][j]).y;
				timerMoveResult_[i][j] = std::fabs(timerMoveResult_[i][j]) / 3200;

				if (timerMoveResult_[i][j] > 0.95f)
				{
					timerMoveResult_[i][j] = 0.95f;
				}
				else if (timerMoveResult_[i][j] < 0.0f)
				{
					timerMoveResult_[i][j] = 0.01;
				}

				if (elecWaitAlpha_[i][j] > 0.95f)
				{
					elecWaitAlpha_[i][j] = 0.95f;

				}
				else if (elecWaitAlpha_[i][j] < 0.0f)
				{
					elecWaitAlpha_[i][j] = 0.01;
				}
				else if (elecWaitAlpha_[i][j] >= 0.0f && elecWaitAlpha_[i][j] < 0.95f)
				{
					elecWaitAlpha_[i][j] += timerMoveResult_[i][j];
				}
			}

			if (isPopWait == true && isStartPop == false)
			{
				Vec3 start{ -20.0f, 0.0f, 0.0f };
				Vec3 p2{ 5.0f, 160.0f, 0.0f };
				Vec3 p3{ 5.0f, 40.0f, 0.0f };
				Vec3 end{ +20.0f, 0.0f, 0.0f };

				Vec3 a = LerpVec3(start, p2, timeRate[i][j]);
				Vec3 b = LerpVec3(p2, p3, timeRate[i][j]);

				timerMoveResult_[i][j] = LerpVec3(a, b, timeRate[i][j]).y;
				timerMoveResult_[i][j] = std::fabs(timerMoveResult_[i][j]) / 1000;

				if (timerMoveResult_[i][j] > 0.95f)
				{
					timerMoveResult_[i][j] = 0.95f;
				}
				else if (timerMoveResult_[i][j] < 0.0f)
				{
					timerMoveResult_[i][j] = 0.01;
				}

				if (elecWaitAlpha_[i][j] > 0.95f)
				{
					elecWaitAlpha_[i][j] = 0.95f;

				}
				else if (elecWaitAlpha_[i][j] < 0.0f)
				{
					elecWaitAlpha_[i][j] = 0.01;
				}
				else if (elecWaitAlpha_[i][j] >= 0.0f && elecWaitAlpha_[i][j] < 0.95f)
				{
					elecWaitAlpha_[i][j] += timerMoveResult_[i][j];
				}
			}
		}
	}

	if (elecWaitAlpha_[stageWidth_][stageHeight_] >= 0.95f && isStartPop == true)
	{
		isStartPop = false;
	}

	if (elecWaitAlpha_[stageWidth_][stageHeight_] >= 0.95f && isPopWait == true)
	{
		isPopWait = false;
	}
}

void BlockManager::UpdateGoalConnect()
{

}


//重なった時の処理
void BlockManager::UpdateOverlap()
{
	int prevBlockY = 0;
	int prevBlockX = 0;

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (form_[i][j] == Form::Electric)
			{
				isElec[i][j] = true;
			}

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

							if (form_[i][j] != Form::NONE && form_[k][l] != Form::NONE && form_[i][j] != Form::BUTTON && form_[k][l] != Form::BUTTON)
								//if (form_[i][j] != Form::GOAL && form_[k][l] != Form::GOAL)
							{
								//重なっているブロック両方を固定ブロック化

								//ボタンを押さない回転の処理
								//回転させる前の状態を保存
								if (isTurn[i][j] == false || isTurn[k][l] == false)
								{
									if (form_[i][j] == Form::GOAL || form_[k][l] == Form::GOAL)
									{
										isPopGoal = false;
										//isConectedGoal = false;
									}

									beforeTurn_[i][j] = form_[i][j];
									beforeTurn_[k][l] = form_[k][l];

									form_[i][j] = Form::LOCKED;
									form_[k][l] = Form::LOCKED;

									if (isUp[i][j] == true)
									{
										//ブロックの演出
										blocks_[i][j]->GetWorldTransForm()->scale =
										{ blocks_[i][j]->GetRadius() * 1.8f,blocks_[i][j]->GetRadius() * 1.8f, blocks_[i][j]->GetRadius() * 1.8f };
									}
									else if (isUp[k][l] == true)
									{
										//ブロックの演出
										blocks_[k][l]->GetWorldTransForm()->scale =
										{ blocks_[k][l]->GetRadius() * 1.8f,blocks_[k][l]->GetRadius() * 1.8f, blocks_[k][l]->GetRadius() * 1.8f };
									}
								}

								////重なったところを電気が通るように
								//if(isElec[i][j] == true && isElec[k][l] == false)
								//{
								//	isElec[k][l] = true;
								//}

								//重なったところを固定状態に
								form_[i][j] = Form::LOCKED;
								form_[k][l] = Form::LOCKED;
								//回転したフラグをONに
								isTurn[i][j] = true;
								isTurn[k][l] = true;


							}
							//ボタンを押したときの処理
							else if (form_[i][j] == Form::BUTTON && form_[k][l] != Form::GOAL)
							{
								if (isTurn[i][j] == false || isTurn[k][l] == false)
								{
									//ボタンの形はそのままで重なった方を
									beforeTurn_[i][j] = Form::BUTTON;
									beforeTurn_[k][l] = form_[k][l];

									form_[i][j] = Form::BUTTON;
									form_[k][l] = Form::LOCKED;

									//押したボタンに電気が通らないように
									isElec[i][j] = false;
									isElec[k][l] = false;

									if (isUp[i][j] == true)
									{
										//ブロックの演出
										blocks_[i][j]->GetWorldTransForm()->scale =
										{ blocks_[i][j]->GetRadius() * 1.8f,blocks_[i][j]->GetRadius() * 1.8f, blocks_[i][j]->GetRadius() * 1.8f };
									}
									else if (isUp[k][l] == true)
									{
										//ブロックの演出
										blocks_[k][l]->GetWorldTransForm()->scale =
										{ blocks_[k][l]->GetRadius() * 1.8f,blocks_[k][l]->GetRadius() * 1.8f, blocks_[k][l]->GetRadius() * 1.8f };
									}
								}

								////重なったところを電気が通るように
								//if(isElec[i][j] == true && isElec[k][l] == false)
								//{
								//	isElec[k][l] = true;
								//}

								//重なったところを固定状態に
								form_[i][j] = Form::BUTTON;
								form_[k][l] = Form::LOCKED;
								//回転したフラグをONに
								isTurn[i][j] = true;
								isTurn[k][l] = true;

								/*if(isTurn[i][j] == true)
								{
									isElec[i][j] = false;
								}
								if(isTurn[k][l] == true)
								{
									isElec[k][l] = false;
								}*/

								//押したフラグをONに
								if (isPushed[i][j] == false)
								{
									isPushed[i][j] = true;
									//押された数を増やす
									pushedCount_++;

									//音
									Sound::GetInstance().PlayWave("button (2).wav", 1.2f);
								}

							}

							//チュートリアル
							if (tutorial->GetState() == TUTORIAL::OVERLAP)
							{
								//tutorial->AddStateNum();
							}
						}
					}

					//もし[i][j]が[k][l]番のブロックと隣接していたら
					//if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == true)
					//{
					//	//同じ座標ではないとき
					//	if (i != k || j != l)
					//	{
					//		if (isElec[i][j] == true && form_[k][l] != Form::NONE && form_[k][l] != Form::LOCKED && isTurn[k][l] == false)
					//		{
					//			isElec[k][l] = true;
					//			//isDecisionElec[k][l] = true;
					//		}
					//		else if (form_[k][l] == Form::NONE || form_[k][l] == Form::LOCKED || isTurn[k][l] == true)
					//		{
					//			isElec[k][l] = false;
					//			//isDecisionElec[k][l] = false;
					//		}
					//		else if (form_[k][l] == Form::BUTTON && isPushed[k][l] == true)
					//		{
					//			isElec[k][l] = false;
					//		}

					//		/*else if(isTurn[i][j] == true)
					//		{
					//			isElec[i][j] = false;
					//		}
					//		else if(isTurn[k][l] == true)
					//		{
					//			isElec[k][l] = false;
					//		}*/



					//	}
					//}

					//if (form_[i][j] == Form::GOAL)
					//{
					//	//同じ座標ではないとき
					//	if (i != k || j != l)
					//	{
					//		if (form_[k][l] != Form::NONE && form_[k][l] != Form::LOCKED)
					//		{
					//			//どこかがゴールにつながっていれば、繋げたままにする
					//			if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == false)
					//			{
					//				if (isChangedConectGoal == false)
					//				{
					//					isConectedGoal = false;
					//				}
					//			}
					//			else
					//			{
					//				isConectedGoal = true;
					//				isChangedConectGoal = true;
					//			}
					//		}
					//	}
					//}

					/*if(i == stageWidth_ - 1 && j == stageWidth_ - 1 && k == stageWidth_ - 1 && l == stageWidth_ - 1)
					{
						checkCount++;
					}*/
				}
			}

			//Y座標の一つ前の番号を保存
			if (prevBlockY >= stageHeight_ - 2)
			{
				prevBlockY = 0;
			}
			else
			{
				prevBlockY = j;
			}
		}
		//X座標の一つ前のブロック番号を保存
		if (prevBlockX >= stageWidth_ - 1)
		{
			prevBlockX = 0;
		}
		else
		{
			prevBlockX = i;
		}
	}

	//for文のストッパーをリセットする
	isChangedConectGoal = false;


	/*if(checkCount > 11)
	{
		isCheckElec_ = true;
	}*/
}

void BlockManager::ConectElec()
{


	//一回の更新処理
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (form_[i][j] == Form::Electric)
			{
				isElec[i][j] = true;
			}

			for (int k = 0; k < stageWidth_; k++)
			{
				for (int l = 0; l < stageHeight_; l++)
				{
					//もし[i][j]が[k][l]番のブロックと隣接していたら
					if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == true)
					{
						//同じ座標ではないとき
						if (i != k || j != l)
						{
							if (isElec[i][j] == true && form_[k][l] != Form::NONE && form_[k][l] != Form::LOCKED && isTurning[k][l] == false && isTurn[k][l] == false
								&& isElec[k][l] == false)
							{
								isElec[k][l] = true;
								//isDecisionElec[k][l] = true;

								if (form_[k][l] == Form::GOAL)
								{
									//音
									Sound::GetInstance().PlayWave("connectGoal.wav", 0.6f);

									//パーティクル発生
									ParticleManager::GetInstance()->GenerateRandomParticle(40, 100, 0.3f,
										{ worldmats_[k][l].trans.x,worldmats_[k][l].trans.y + blockRadius_ * 2.0f,worldmats_[k][l].trans.z },
										2.0f, 0, { 0.1f,0.2f,1.0f,1.0f }, { 1.0f,0.0f,0.0f,1.0f });

									//ロックオン画像演出
									rockOnImage->BeginEffect({ worldmats_[k][l].trans.x,worldmats_[k][l].trans.y + blockRadius_ * 2.0f,worldmats_[k][l].trans.z });
								}
							}
							else if (form_[k][l] == Form::NONE || form_[k][l] == Form::LOCKED || isTurning[k][l] == true)
							{
								isElec[k][l] = false;
								//isDecisionElec[k][l] = false;
							}
							else if (form_[i][j] == Form::BUTTON && isPushed[i][j] == true)
							{
								isElec[i][j] = false;
							}
							else if (form_[k][l] == Form::BUTTON && isPushed[k][l] == true)
							{
								isElec[k][l] = false;
							}
						}
					}

					if (i == stageWidth_ - 1 && j == stageWidth_ - 1 && k == stageWidth_ - 1 && l == stageWidth_ - 1)
					{
						checkCount++;
					}
				}
			}
		}
	}



	if (checkCount > 11)
	{

		isCheckElec_ = true;
	}
}

void BlockManager::GeneratePredictBlock()
{
	predictBlockM->ClearPredictBlock();

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			if (this->action_[i][j] != Action::Connect)
			{
				continue;
			}

			//右開店と左回転両方
			WorldMat worldMat[2];
			worldMat[0].rot.y = pi / 2.0f;
			worldMat[0].SetWorld();
			worldMat[1].rot.y = pi / -2.0f;
			worldMat[1].SetWorld();

			//ブロックの回転
			worldMat[0].trans.x = axis_pos_.x + GetVec3xM4(distancePos[i][j], worldMat[0].matWorld, 0).x;
			worldMat[0].trans.y = 0.8f;
			worldMat[0].trans.z = axis_pos_.z + GetVec3xM4(distancePos[i][j], worldMat[0].matWorld, 0).z;
			worldMat[1].trans.x = axis_pos_.x + GetVec3xM4(distancePos[i][j], worldMat[1].matWorld, 0).x;
			worldMat[1].trans.y = 0.8f;
			worldMat[1].trans.z = axis_pos_.z + GetVec3xM4(distancePos[i][j], worldMat[1].matWorld, 0).z;

			this->electricBlock = electricBlock; this->doorGoalClosed = doorGoalClosed;

			if (form_[i][j] == Form::BLOCK)
			{
				predictBlockM->AddPredictBlock(worldMat[0].trans, { blockRadius_,blockRadius_,blockRadius_ }, false, this->disconnectedBlock);
				predictBlockM->AddPredictBlock(worldMat[1].trans, { blockRadius_,blockRadius_,blockRadius_ }, true, this->disconnectedBlock);
			}
			if (form_[i][j] == Form::LOCKED)
			{
				predictBlockM->AddPredictBlock(worldMat[0].trans, { blockRadius_,blockRadius_,blockRadius_ }, false, this->locked);
				predictBlockM->AddPredictBlock(worldMat[1].trans, { blockRadius_,blockRadius_,blockRadius_ }, true, this->locked);
			}
			if (form_[i][j] == Form::GOAL)
			{
				predictBlockM->AddPredictBlock(worldMat[0].trans, { blockRadius_,blockRadius_,blockRadius_ }, false, this->goal);
				predictBlockM->AddPredictBlock(worldMat[1].trans, { blockRadius_,blockRadius_,blockRadius_ }, true, this->goal);
			}
			if (form_[i][j] == Form::GEAR)
			{
				predictBlockM->AddPredictBlock(worldMat[0].trans, { blockRadius_,blockRadius_,blockRadius_ }, false, this->Socket);
				predictBlockM->AddPredictBlock(worldMat[1].trans, { blockRadius_,blockRadius_,blockRadius_ }, true, this->Socket);
			}
			if (form_[i][j] == Form::BUTTON)
			{
				predictBlockM->AddPredictBlock(worldMat[0].trans, { blockRadius_,blockRadius_,blockRadius_ }, false, this->button);
				predictBlockM->AddPredictBlock(worldMat[1].trans, { blockRadius_,blockRadius_,blockRadius_ }, true, this->button);
			}
			if (form_[i][j] == Form::Electric)
			{
				predictBlockM->AddPredictBlock(worldMat[0].trans, { blockRadius_,blockRadius_,blockRadius_ }, false, this->electricBlock);
				predictBlockM->AddPredictBlock(worldMat[1].trans, { blockRadius_,blockRadius_,blockRadius_ }, true, this->electricBlock);
			}
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
									if (form_[k][l] == Form::BUTTON)
									{
										form_[i][j] = beforeTurn_[i][j];

										/*if(isTurn[i][j] == true)
										{
											isElec[i][j] = false;
										}
										if(isTurn[k][l] == true)
										{
											isElec[k][l] = false;
										}*/


										//回転したフラグをOFFに
										isTurn[i][j] = false;
										isTurn[k][l] = false;


										//押したフラグをOFFに
										if (isPushed[k][l] == true)
										{
											//押された数を減らす
											if (pushedCount_ > 0)
											{
												pushedCount_--;
											}
											isPushed[k][l] = false;
										}
									}
									else if (form_[k][l] != Form::NONE)
									{
										//回転する前の状態に戻す
										form_[i][j] = beforeTurn_[i][j];
										form_[k][l] = beforeTurn_[k][l];

										if (form_[i][j] == Form::GOAL || form_[k][l] == Form::GOAL)
										{
											isPopGoal = true;

										}

										/*if(isTurn[i][j] == true)
										{
											isElec[i][j] = false;
										}
										if(isTurn[k][l] == true)
										{
											isElec[k][l] = false;
										}*/


										//回転したフラグをOFFに
										isTurn[i][j] = false;
										isTurn[k][l] = false;


									}
								}
							}
						}
					}

					if (form_[i][j] == Form::GOAL)
					{
						//同じ座標ではないとき
						if (i != k || j != l)
						{
							if (form_[k][l] != Form::NONE && form_[k][l] != Form::LOCKED)
							{
								//どこかがゴールにつながっていれば、繋げたままにする
								if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == false)
								{
									if (isChangedConectGoal == false)
									{
										isConectedGoal = false;
									}
								}
								else
								{
									isConectedGoal = true;
									isChangedConectGoal = true;
								}
							}
						}
					}
				}
			}
		}
	}

	//for文のストッパーをリセットする
	isChangedConectGoal = false;

}

//電気ブロックからの電気をつける初期化
void BlockManager::InitializeElectric()
{
	int prevBlockX = 0;
	int prevBlockY = 0;

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//電気ブロックのところを電気が通っているように
			if (form_[i][j] == Form::Electric)
			{
				isElec[i][j] = true;
			}

			if (isElec[i][j] == true)
			{
				//電気が通ってるブロックから見て、電気を通せるブロックがあるなら電気を通す
				if (form_[i + 1][j] != Form::NONE && form_[i + 1][j] != Form::LOCKED)
				{
					//通っていなかったら
					if (isElec[i + 1][j] == false)
					{
						isElec[i + 1][j] = true;
						/*if(form_[i + 1][j] != Form::BUTTON && form_[i + 1][j] != Form::GEAR)
						{
							form_[i + 1][j] = Form::Electric;
						}*/
						//form_[i + 1][j] = Form::Electric;
					}
				}
				else if (form_[i + 1][j] == Form::NONE || form_[i + 1][j] == Form::LOCKED)
				{
					isElec[i + 1][j] = false;
				}
				//←左方向の処理(電気フラグを変える処理)

				if (form_[prevBlockX][j] != Form::NONE && form_[prevBlockX][j] != Form::LOCKED)
				{
					if (isElec[prevBlockX][j] == false)
					{
						isElec[prevBlockX][j] = true;

						/*if(form_[prevBlockX][j] != Form::BUTTON && form_[prevBlockX][j] != Form::GEAR)
						{
							form_[prevBlockX][j] = Form::Electric;
						}*/
						//form_[prevBlockX][j] = Form::Electric;
					}
				}

				else if (form_[prevBlockX][j] == Form::NONE || form_[prevBlockX][j] == Form::LOCKED)
				{
					isElec[prevBlockX][j] = false;
				}

				//↑上方向の処理(電気フラグを変える処理)
				if (form_[i][j + 1] != Form::NONE && form_[i][j + 1] != Form::LOCKED)
				{

					if (isElec[i][j + 1] == false)
					{
						isElec[i][j + 1] = true;
						/*if(form_[i][j + 1] != Form::BUTTON && form_[i][j + 1] != Form::GEAR)
						{
							form_[i][j + 1] = Form::Electric;
						}*/
						//form_[i][j + 1] = Form::Electric;
					}

				}
				else if (form_[i][j + 1] == Form::NONE || form_[i][j + 1] == Form::LOCKED)
				{
					isElec[i][j + 1] = false;
				}

				//↓下方向の処理(電気フラグを変える処理)
				if (form_[i][prevBlockY] != Form::NONE && form_[i][prevBlockY] != Form::LOCKED)
				{

					if (isElec[i][prevBlockY] == false)
					{
						isElec[i][prevBlockY] = true;
						/*if(form_[i][prevBlockY] != Form::BUTTON && form_[i][prevBlockY] != Form::GEAR)
						{
							form_[i][prevBlockY] = Form::Electric;
						}*/
						//form_[i][prevBlockY] = Form::Electric;
					}
				}
				else if (form_[i][prevBlockY] == Form::NONE || form_[i][prevBlockY] == Form::LOCKED)
				{
					isElec[i][prevBlockY] = false;
				}

			}

			//どこも電気が繋がっていなかったらOFFにする
			if (isElec[i][prevBlockY] == false && isElec[i][j + 1] == false && isElec[prevBlockX][j] == false && isElec[i + 1][j] == false)
			{
				isElec[i][j] = false;
			}

			if (prevBlockY >= stageHeight_ - 2)
			{
				prevBlockY = 0;
			}
			else
			{
				prevBlockY = j;
			}
		}

		//Y座標の一つ前のブロック番号を保存
		if (prevBlockX >= stageWidth_ - 1)
		{
			prevBlockX = 0;
		}
		else
		{
			prevBlockX = i;
		}
	}

	//電気ブロックの位置から繋げているので
	//その前の[i][j]では値がtrueにならず
	//次のフレームでONにしているので
	//再度for文を繰り返す。
}

void BlockManager::AppearGoal()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//もしボタンが押されていたら
			//ゴールを出現させる
			if (pushedCount_ >= needGoalCount && isPopGoal == false)
			{
				if (isGoal_[i][j] == true && form_[i][j] != Form::GOAL)
				{
					if (isTurn[i][j] == false)
					{
						isPopGoal = true;

						goalPopX = i; goalPopY = j;

						//ステージで一回のみ(電気を待ってから)
						if (!isPopedGoal)
						{
							//カメラ演出
							Vec3 goalPos = worldmats_[i][j].trans;

							cameraM->BegineLerpUsingCamera(cameraM->gameTurnCamera->GetEye(),
								{ goalPos.x,goalPos.y + blockRadius_ * 4.0f,goalPos.z - blockRadius_ * 8.0f },
								cameraM->gameTurnCamera->GetTarget(),
								{ goalPos.x,goalPos.y + blockRadius_ * 2.0f,goalPos.z },
								cameraM->gameTurnCamera->GetUp(),
								{ 0,1.0f,0 },
								90,
								cameraM->gameTurnCamera.get(),
								isPopedGoalCountTmp
							);
							cameraM->usingCamera = cameraM->goalEffectCamera.get();
							cameraM->Update();
							cameraM->usingCamera->CameraShake(30, 1.0f);

							isPopedGoal2 = true;
						}
						else
						{
							//カメラ演出
							Vec3 goalPos = worldmats_[i][j].trans;

							cameraM->BegineLerpUsingCamera(cameraM->gameTurnCamera->GetEye(),
								cameraM->gameTurnCamera->GetEye(),
								cameraM->gameTurnCamera->GetTarget(),
								cameraM->gameTurnCamera->GetTarget(),
								cameraM->gameTurnCamera->GetUp(),
								cameraM->gameTurnCamera->GetUp(),
								2,
								cameraM->gameTurnCamera.get(),
								1
							);

							isPopedGoal2 = false;
						}

						isPopGoalEffect = true;


						////チュートリアル
						//if (tutorial->GetState() == TUTORIAL::BUTTON && tutorial->GetStateNum() == 0)
						//{
						//	tutorial->spriteCount = 0;
						//	tutorial->AddStateNum();
						//}
					}
				}
			}

			else if (pushedCount_ < needGoalCount)
			{
				if (isGoal_[i][j] == true)
				{
					form_[i][j] = Form::LOCKED;
					isConectedGoal = false;
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

//隣接しているかどうか
bool BlockManager::BlockJunction(Vec3 Pos1, Vec3 Pos2)
{
	//誤差修正用
	float EPSILON = 0.005f;

	//右(Pos2が右)
	//上(Pos2が上)
	float resultX = (Pos2.x) - (Pos1.x);
	float resultZ = (Pos2.z) - (Pos1.z);

	//各方向の距離
	float distanceX = std::fabs(resultX);
	float distanceZ = std::fabs(resultZ);

	//中心座標との距離
	Vec3 disvec = { distanceX,0,distanceZ };
	float distance = disvec.GetLength();

	//斜めの対策用に距離で比較
	if (distance <= blockRadius_ * 2 + EPSILON)
	{
		return true;
	}

	return false;

}

void BlockManager::ResetBlock()
{
	needGoalCount = 0;
	//ステージ出現用のフラグ
	isPopWait = true;
	isStartPop = false;

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			worldmats_[i][j] = loadWorldmats_[i][j];

			//ブロックの座標を設定
			form_[i][j] = loadForms_[i][j];

			if (form_[i][j] == Form::BUTTON)
			{
				needGoalCount++;
				isElec[i][j] = false;
			}
			else if (form_[i][j] == Form::Electric)
			{
				isElec[i][j] = true;
			}
			else
			{
				isElec[i][j] = false;
			}

			//ゴールのワールド行列
			if (form_[i][j] == Form::GOAL)
			{
				goalMat = worldmats_[i][j];
				goalMat.scale = { Vec3(blockRadius_,blockRadius_,blockRadius_) };
			}

			isGoal_[i][j] = false;

			worldmats_[i][j] = loadWorldmats_[i][j];

			worldmats_[i][j].SetWorld();

			//軸になっているかどうか
			isAxis_[i][j] = false;

			//現在どうなっているか
			action_[i][j] = Action::None;
			//回転させる前の状態に、読み込んだ状態を入れる
			beforeTurn_[i][j] = form_[i][j];
			//押されているかどうか
			isPushed[i][j] = false;

			//Y座標を浮かせるフラグを初期化
			isUp[i][j] = false;

			isDecisionElec[i][j] = false;

			isTurn[i][j] = false;

			isTurning[i][j] = false;

			startIndex_[i][j] = 1;
			timeRate[i][j] = 0;
			elecCount_[i][j] = 0;
			elecWaitAlpha_[i][j] = 0.01;
			timerMoveResult_[i][j] = 0.0f;
		}
	}

	//ボタンのあるステージと分ける場合、場合分け用
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//ボタンがあればゴールを隠す
			if (needGoalCount > 0)
			{
				if (form_[i][j] == Form::GOAL)
				{
					isPopGoal = false;
					isGoal_[i][j] = true;
					form_[i][j] = Form::LOCKED;
				}
			}
		}
		//電気の初期化
		InitializeElectric();
	}

	isConectedGoal = false;
	isChangedConectGoal = false;

	//繋がっているフラグ初期化
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			for (int k = 0; k < stageWidth_; k++)
			{
				for (int l = 0; l < stageHeight_; l++)
				{
					//同じ番号を防ぐ
					if (i == k && j == l) {}
					else
					{
						if (form_[i][j] == Form::GOAL)
						{
							//もし[i][j]がゴールの時に[k][l]番のブロックと隣接していたら
							if (BlockJunction(worldmats_[i][j].trans, worldmats_[k][l].trans) == true)
							{
								isConectedGoal = true;
								isChangedConectGoal = true;
							}
						}
					}

				}
			}
		}
	}

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//もしゴールに電気が通っていて道が繋がっているなら
			if (isElec[i][j] == true && form_[i][j] == Form::GOAL && isConectedGoal == true)
			{
				isStopElecConectedGoal = true;
				//ゴールできるフラグON
				isElecConectedGoal = true;
			}
			else if (isStopElecConectedGoal == false)
			{
				//ゴールできるフラグON
				isElecConectedGoal = false;
			}
		}
	}

	//for文のストッパーをリセットする
	if (isStopElecConectedGoal == true)
	{
		isStopElecConectedGoal = false;
	}

	//回転
	isRightRolling = false;
	isLeftRolling = false;

	isCheckElec_ = false;

	rotateCount = 0;

	angle_ = 0;

	effectCount = 0;

	pushedCount_ = 0;



	//電気初期化用
	checkCount = 0;

	//リセットは戻す
	isPopedGoal = false;
	isPopedGoal2 = false;
	isPopedGoal2Count = 0;

	this->isPopGoalEffect = false;
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
					if (isUp[i][j] == false)
					{
						/*blocks_[i][j]->GetWorldTransForm()->trans =
						{ blocks_[i][j]->GetRadius() * 1.1f,blocks_[i][j]->GetRadius() * 1.1f, blocks_[i][j]->GetRadius() * 1.1f };*/

						beforeTransY[i][j] = worldmats_[i][j].trans.y;
						worldmats_[i][j].trans.y -= 0.20f;

						isUp[i][j] = true;
					}
				}
			}
		}
	}
}
void BlockManager::DownPosY()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (action_[i][j] != Action::Connect)
			{
				if (form_[i][j] == Form::BLOCK || form_[i][j] == Form::GEAR)
				{
					if (isUp[i][j] == true)
					{

						beforeTransY[i][j] = worldmats_[i][j].trans.y;
						worldmats_[i][j].trans.y += 0.20f;

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
	needGoalCount = 0;
	isPopWait = false;
	isStartPop = true;

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

			if (form_[i][j] == Form::BUTTON)
			{
				needGoalCount++;
				isElec[i][j] = false;
			}
			else if (form_[i][j] == Form::Electric)
			{
				isElec[i][j] = true;
			}
			else
			{
				isElec[i][j] = false;
			}

			//ゴールのワールド行列
			if (form_[i][j] == Form::GOAL)
			{
				goalMat = worldmats_[i][j];
				goalMat.scale = { Vec3(blockRadius_,blockRadius_,blockRadius_) };
			}

			isGoal_[i][j] = false;

			//引数で受け取った形状を保存。
			//上記の項目はリセットの際に再設定
			loadForms_[i][j] = forms[i][j];

			isDecisionElec[i][j] = false;

			isTurning[i][j] = false;
		}
	}

	//ボタンのあるステージと分ける場合、場合分け用
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (needGoalCount > 0)
			{
				if (form_[i][j] == Form::GOAL)
				{
					//ボタンのあるステージと分ける場合、場合分け必要
					isPopGoal = false;
					isGoal_[i][j] = true;
					form_[i][j] = Form::LOCKED;
				}
			}
		}
	}
}

void BlockManager::SetElec(int elec[13][13])
{

	//全ての壁の座標を渡す
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//ワールド行列の平行移動成分を取得(ワールド座標)
			//blockPos[i][j] = worldTransforms_[i][j].translation_;
			elec[i][j] = isElec[i][j];
		}
	}

}


//
//void BlockManager::LoadBlockPosData()
//{
//	//ファイルを開く
//	std::ifstream file;
//	file.open("Resources\\blockPos.csv");
//	assert(file.is_open());
//
//	//ファイルの内容を文字列ストリームにコピー
//	blocksPos << file.rdbuf();
//
//	//ファイルを閉じる
//	file.close();
//}
//
//void BlockManager::BlockPop(Vec3 pos)
//{
//	//敵の生成
//
//	//ベクタ配列に要素<ブロック>を追加
//	for (int i = 0; i < stageWidth_; i++)
//	{
//		//ブロック型を持てる空のベクタを追加(行列でいうi列)
//		blocks_.push_back(vector<Block*>());
//
//		for (int j = 0; j < stageHeight_; j++)
//		{
//			block_ = new Block;
//			//ブロックの要素を追加
//			blocks_[i].push_back(block_);
//		}
//	}
//
//	for (int i = 0; i < blockWidth; i++)
//	{
//		for (int j = 0; j < blockHeight; j++)
//		{
//			worldmats_[i][j].trans = pos;
//		}
//	}
//}
//
//void BlockManager::UpdateBlockPos()
//{
//	//待機処理
//	if (isWaitBlock)
//	{
//		blockWaitTimer--;
//		if (blockWaitTimer <= 0)
//		{
//			//待機完了
//			isWaitBlock = false;
//		}
//		return;
//	}
//
//	//1行分の文字列を入れる変数
//	std::string line;
//
//	//コマンド実行ループ
//	while (getline(blocksPos, line))
//	{
//		//1行分の文字列をストリームに変換して解析しやすくする
//		std::istringstream line_stream(line);
//
//		std::string word;
//		//,区切りで行の先頭文字列を取得
//		getline(line_stream, word, ',');
//
//		//"//"から始まる行はコメント
//		if (word.find("//") == 0)
//		{
//			//コメント行を飛ばす
//			continue;
//		}
//
//		//POPコマンドcsv
//		if (word.find("POP") == 0)
//		{
//			//X座標
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//			//Y座標
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//			//Z座標
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//			//ブロックを発生させる
//			BlockPop(Vec3(x, y, z));
//		}
//		//WAITコマンド
//		else if (word.find("WAIT") == 0)
//		{
//			getline(line_stream, word, ',');
//
//			//待ち時間
//			int32_t waitTime = atoi(word.c_str());
//
//			//待機開始
//			isWaitBlock = true;
//			blockWaitTimer = waitTime;
//
//			//コマンドループを抜ける
//			break;
//		}
//	}
//
//}