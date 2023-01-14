#include "BlockManager.h"

using namespace std;

BlockManager::~BlockManager()
{

	//ブロックの削除
	//blocks_.clear();
	delete block_;
	//delete worldmat_;
}

//初期化
void BlockManager::Initialize()
{

	//std::unique_ptr<Block> newBullet = std::make_unique<Block>();

	block_ = new Block;

	//ベクタ配列に要素<ブロック>を追加
	for (int i = 0; i < blockWidth; i++)
	{
		//ブロック型を持てる空のベクタを追加(行列でいうi列)
		blocks_.push_back(vector<Block*>());
		
		for (int j = 0; j < blockHeight; j++)
		{
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
			
			worldmats_[i][j].scale = { 1.8f,1.8f,1.8f };
			
			//worldmats_[i][j]->rot = { 0.0f,0.0f,0.0f };


			blocks_[i][j]->Initialize();
			
			//ブロックの種類を設定
			if (i == 1 && j == 1)
			{
				form_[i][j] = Form::BUTTON;
			}
			else
			{
				form_[i][j] = Form::BLOCK;
			}

			
			//ブロックの座標を設定
			if (i >= 0)
			{
				//blocks_[i][j]->GetWorldTransForm()->trans.x = i * (scale_.x * 1);
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 1);
				
			}
			if (j >= 0)
			{
				//blocks_[i][j]->GetWorldTransForm()->trans.y = i * (scale_.y * 1);

				worldmats_[i][j].trans.y = j * (worldmats_[i][j].scale.y * 1);
			}

			block_->Initialize();
			
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
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//状態
			//preWorldTransform_[i][j] = worldTransform_[i][j];

			//transforms[i][j].x += 0.001f;
			
			//ブロックの更新
			//blocks_[i][j]->SetWorldPos(transforms[i][j]);
			//blocks_[i][j]->Updata(transforms[i][j]);
			/*worldmats_[i][j]->trans.x += 0.001f;
			worldmats_[i][j]->trans.z += 0.001f * j;*/

			//worldmats_[i][j]->SetWorld();
			
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
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			blocks_[i][j]->SetWorldPos(worldmats_[i][j].trans);
			//draw->DrawCube3D(worldmats_[i][j], &camera->viewMat, &camera->projectionMat);
			blocks_[i][j]->Draw(camera);
		}
	}
}

bool BlockManager::CheckPlayerOnBlock(Vec3 pos)
{
	bool result;

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーがブロックの上にいるかどうか
			if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
				&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
			{
				result =  true;
			}
			else
			{
				result =  false;
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
			if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
				&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
			{
				//そのブロックの形状は普通のブロックかどうか
				if (form_[i][j] == Form::BLOCK)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			//プレイヤーがどのブロックにもいない場合
			else
			{
				return false;
			}
		}
	}
}

//ボタンがあるかどうか
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//プレイヤーが指定のブロックの上にいるかどうか
			if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
				&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
			{
				//そのブロックの形状はボタンかどうか
				if (form_[i][j] == Form::BUTTON)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			//プレイヤーがどのブロックにもいない場合
			else
			{
				return false;
			}
		}
	}
}

//最初に繋ぐボタンを押したブロックを軸に登録する関数
void BlockManager::RegistAxisButton(const Vec3& pos, bool isConnect)
{
	//最初にボタンを押したブロックを軸に登録する関数
	//引数で受け取ったプレイヤーの座標をもとに現在位置のボタンをONにする

	//プレイヤーの位置にあるブロックを軸にする

	if (isConnect == true)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//プレイヤーが指定のブロックの上にいるかどうか
				if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
					&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
				{
					//そのブロックの形状はボタンかどうか
					if (form_[i][j] == Form::BUTTON && isAxis_[i][j] == false)
					{
						//軸登録する
						isAxis_[i][j] = true;
					}
					else{}
				}
				//プレイヤーがどのブロックにもいない場合
				else{}
			}
		}
	}

}

//ブロック同士をつなぐ更新関数
void BlockManager::UpdateConnect(Vec3 pos, bool isConnect)
{
	if (isConnect == true)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//プレイヤーが指定のブロックの上にいるかどうか
				if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
					&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
				{
					//繋ぎはじめ
					//ボタンかつ繋がっていなければ現在位置をつなぐ状態にする
					if (form_[i][j] == Form::BUTTON && action_[i][j] == Action::None)
					{
						action_[i][j] = Action::Connect;
					}

					//繋ぐ処理(右だけテスト)
					if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHT) && i < blockWidth - 1 && changedAction_ == true && isChanged_ == true)
					{
						//前ブロックが繋がっていれば繋げる
						if (action_[i][j] == Action::Connect)
						{
							if (form_[i + 1][j] == Form::BLOCK)
							{
								//ブロックなら繋げる
								action_[i + 1][j] = Action::Connect;
								isChanged_ = false;
							}
							else if (form_[i + 1][j] == Form::GEAR)
							{
								//ギアなら繋げて止める
								action_[i + 1][j] = Action::Connect;
								isChanged_ = false;
								changedAction_ = false;
							}
						}
					}
				}

			}
		}
	}
}

//繋ぐ際に離したところが軸以外のボタンかどうか
bool BlockManager::CheckAxisButton(Vec3 pos, bool isConnect)
{
	if (isConnect == true)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//プレイヤーがいるブロック内において
				if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
					&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
				{
					//現在のブロックが軸登録されているならfalse
					if (isAxis_[i][j] == true )
					{
						return false;
					}
					else  if(isAxis_[i][j] == false)
					{
						return true;
					}
				}
			}
		}
	}
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
			}
		}
	}
}

//キーボードによって回転
//キーボードを関数内で取得
void BlockManager::UpdateRotatePlayerBlock(Vec3& rotatePos, bool isRota)
{
	//軸に向けての方向ベクトルを取得

	//角度が必要(前にやった円運動が参考になるかも)

	if (isRota)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//もしつながっているなら
				if (action_[i][j] == Action::Connect)
				{

				}

				//回転処理
			}
		}
	}
}
