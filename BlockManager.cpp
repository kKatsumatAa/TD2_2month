#include "BlockManager.h"
#include <fstream>
#include "ParticleManager.h"


using namespace std;

BlockManager::~BlockManager()
{

	//�u���b�N�̍폜
	blocks_.clear();
	worldmats_.clear();
	delete block_;
	//delete worldmat_;
}

//������
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

	//�x�N�^�z��ɗv�f<�u���b�N>��ǉ�
	for (int i = 0; i < blockWidth; i++)
	{
		//�u���b�N�^����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		blocks_.push_back(vector<Block*>());

		for (int j = 0; j < blockHeight; j++)
		{
			block_ = new Block;
			//�u���b�N�̗v�f��ǉ�
			blocks_[i].push_back(block_);
		}
	}

	//�x�N�^�z��ɗv�f<���[���h�s��>��ǉ�
	for (int i = 0; i < blockWidth; i++)
	{
		//�u���b�N�^����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		worldmats_.push_back(vector<WorldMat>());

		for (int j = 0; j < blockHeight; j++)
		{
			//�u���b�N�̗v�f��ǉ�
			worldmats_[i].push_back(worldmat_);
		}
	}

	//�u���b�N�̑傫��
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

			worldmats_[i][j].scale = { blockRadius_,blockRadius_,blockRadius_ };

			//worldmats_[i][j]->rot = { 0.0f,0.0f,0.0f };

			blocks_[i][j]->Initialize(connectEM, normal, button, goal, Socket);

			//�u���b�N�̎�ނ�ݒ�

			//�u���b�N�̍��W��ݒ�
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

			//���ɂȂ��Ă��邩�ǂ���
			isAxis_[i][j] = false;

			//���݂ǂ��Ȃ��Ă��邩
			action_[i][j] = Action::None;

			isTurn[i][j] = false;

			beforeTurn_[i][j] = form_[i][j];

		}
	}

	//���̑��̐ݒ�
	isCount = 1;

	changedAction_ = false;
	isChanged_ = false;
	//��]

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


			//X���W�̈�O�̔ԍ���ۑ�
			prevBlockY = j;

			blocks_[i][j]->Updata();

		}
		//Y���W�̈�O�̃u���b�N�ԍ���ۑ�
		prevBlockX = i;
	}

	//��Ԃ�ς��鎞�̒x��
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
			//Manager.cpp�Ŕz��Œ�`����worldTransform�̒l��Block.cpp��Draw�ɃZ�b�g
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
	bool result;

	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
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

//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos�����)
bool BlockManager::GetPosIsBlock(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{

			//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
			if (worldmats_[i][j].trans.x - blockRadius_ <= pos.x && worldmats_[i][j].trans.x + blockRadius_ >= pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ <= pos.z && worldmats_[i][j].trans.z + blockRadius_ >= pos.z)
			{
				//���̃u���b�N�̌`��͕��ʂ̃u���b�N���ǂ���
				if (form_[i][j] != Form::NONE && form_[i][j] != Form::LOCKED && action_[i][j] != Action::Connect)
				{
					return true;
				}

			}
		}
	}

	//player���ǂ̃u���b�N�ɂ���Ȃ�
	return false;
}

//�{�^�������邩�ǂ���
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//���̃u���b�N�̌`��̓{�^�����ǂ���
				if (form_[i][j] == Form::BUTTON)
				{
					return true;
				}
			}
		}
	}

	//�v���C���[���ǂ̃u���b�N�ɂ���Ȃ��ꍇ
	return false;

}

//�ŏ��Ɍq���{�^����������u���b�N����ɓo�^����֐�
void BlockManager::RegistAxisButton(const Vec3& pos)
{
	//�ŏ��Ƀ{�^����������u���b�N����ɓo�^����֐�
	//�����Ŏ󂯎�����v���C���[�̍��W���ƂɌ��݈ʒu�̃{�^����ON�ɂ���

	//�v���C���[�̈ʒu�ɂ���u���b�N����ɂ���
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//���̃u���b�N�̌`��̓{�^�����ǂ���
				if (form_[i][j] == Form::BUTTON && isAxis_[i][j] == false)
				{
					//���o�^����
					isAxis_[i][j] = true;
					//���̃u���b�N�̍��W�𓾂�
					axis_pos_.x = worldmats_[i][j].trans.x;
					axis_pos_.y = worldmats_[i][j].trans.y;
					axis_pos_.z = worldmats_[i][j].trans.z;

					cameraM->usingCamera->CameraShake(15, 1.3f);
				}
				else {}
			}
			//�v���C���[���ǂ̃u���b�N�ɂ���Ȃ��ꍇ
			else {}
		}
	}

}

//�u���b�N���m��Ȃ��X�V�֐�
void BlockManager::UpdateConnect(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if ((worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
				&& action_[i][j] != Action::Connect)
			{
				if (form_[i][j] != Form::NONE)
				{
					action_[i][j] = Action::Connect;
					cameraM->usingCamera->CameraShake(15, 0.53f);

				}

				

				////�q���͂���
				////�{�^�����q�����Ă��Ȃ���Ό��݈ʒu��Ȃ���Ԃɂ���
				//if (form_[i][j] == Form::BUTTON && action_[i][j] == Action::None)
				//{
				//	action_[i][j] = Action::Connect;
				//}

				////�q������(�E�����e�X�g)
				//if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHT) && i < blockWidth - 1)
				//{
				//	//�O�u���b�N���q�����Ă���Όq����
				//	if (action_[i][j] == Action::Connect)
				//	{
				//		if (form_[i + 1][j] == Form::BLOCK)
				//		{
				//			//�u���b�N�Ȃ�q����
				//			action_[i + 1][j] = Action::Connect;
				//			isChanged_ = false;
				//		}
				//		else if (form_[i + 1][j] == Form::GEAR)
				//		{
				//			//�M�A�Ȃ�q���Ď~�߂�
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

//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
bool BlockManager::CheckAxisButton(Vec3 pos)
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			//�v���C���[������u���b�N��ɂ�����
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				if (isAxis_[i][j] == false && form_[i][j] == Form::BUTTON)
				{
					cameraM->usingCamera->CameraShake(15, 1.2f);
					return true;
				}
			}
		}
	}

	//���݂̃u���b�N�����o�^����Ă���Ȃ�false
	return false;
}

//�q����Ă���u���b�N��S���������
void BlockManager::ReleseConectedBlock()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (action_[i][j] == Action::Connect)
			{
				//�S��������Ă��Ȃ���Ԃ�
				action_[i][j] = Action::None;
				isAxis_[i][j] = false;
			}
		}
	}
}

//�L�[�{�[�h�ɂ���ĉ�]

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

		//�p�x���K�v(�O�ɂ�����~�^�����Q�l�ɂȂ邩��)
		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
			{

				//����Ȃ����Ă���Ȃ�
				if (action_[i][j] == Action::Connect && isAxis_[i][j] == false)
				{
					WorldMat worldMat;
					worldMat.rot.y = LerpVec3({ angle_, 0, 0 }, { pi / 2.0f,0,0 },
						EaseOut((float)rotateCount / (float)rotateCountMax)).x;
					worldMat.SetWorld();

					//�u���b�N�̉�]
					worldmats_[i][j].trans.x = axis_pos_.x + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).x;
					worldmats_[i][j].trans.z = axis_pos_.z + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).z;
					worldmats_[i][j].rot.y = worldMat.rot.y;
					worldmats_[i][j].SetWorld();

					//�v���C���[�̉�]
					rotatePos.x = axis_pos_.x + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).x;

					rotatePos.z = axis_pos_.z + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).z;
				}
			}
		}


		if (rotateCount >= rotateCountMax)
		{
			isRightRolling = false;
			cameraM->usingCamera->CameraShake(15, 1.5f);

			//�`���[�g���A��
			if (tutorial->GetState() == TUTORIAL::TURN)
			{
				tutorial->AddStateNum();
			}

			//�p�[�e�B�N������
			GenerateParticleTurnBlock();
		}

	}

	if (isLeftRolling == true)
	{
		rotateCount++;

		//�p�x���K�v(�O�ɂ�����~�^�����Q�l�ɂȂ邩��)
		for (int i = 0; i < stageWidth_; i++)
		{
			for (int j = 0; j < stageHeight_; j++)
			{
				//����Ȃ����Ă���Ȃ�
				if (action_[i][j] == Action::Connect && isAxis_[i][j] == false)
				{
					WorldMat worldMat;
					worldMat.rot.y = LerpVec3({ angle_, 0, 0 }, { -pi / 2.0f,0,0 },
						EaseOut((float)rotateCount / (float)rotateCountMax)).x;
					worldMat.SetWorld();

					//�u���b�N�̉�]
					worldmats_[i][j].trans.x = axis_pos_.x + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).x;
					worldmats_[i][j].trans.z = axis_pos_.z + GetVec3xM4(distancePos[i][j], worldMat.matWorld, 0).z;
					worldmats_[i][j].rot.y = worldMat.rot.y;

					//�v���C���[�̉�]
					rotatePos.x = axis_pos_.x + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).x;

					rotatePos.z = axis_pos_.z + GetVec3xM4(distancePosPlayer, worldMat.matWorld, 0).z;

				}
			}
		}


		if (rotateCount >= rotateCountMax)
		{
			isLeftRolling = false;
			cameraM->usingCamera->CameraShake(15, 1.5f);

			//�`���[�g���A��
			if (tutorial->GetState() == TUTORIAL::TURN)
			{
				tutorial->AddStateNum();
			}

			//�p�[�e�B�N������
			GenerateParticleTurnBlock();
		}
	}

	if (isLeftRolling == false && isRightRolling == false)
	{

		UpdateOverlap();
	}

	else if (isLeftRolling == true || isRightRolling == true)
	{
		ChangePosY();
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
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//���̃u���b�N�̌`��͂��S�[�����ǂ���
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

	//�v���C���[���ǂ̃u���b�N�ɂ���Ȃ��ꍇ
	return false;
}

//�d�Ȃ������̏���
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
					//�d�Ȃ������Ɍ`��ς��鏈��
					if (CollisionBlockToBlock(worldmats_[i][j].trans, worldmats_[k][l].trans) && form_[k][l] != Form::NONE && form_[i][j] != Form::NONE)
					{
						//�������W�ł͂Ȃ��Ƃ�
						if (i != k || j != l)
						{
							/*beforeTurn_[i][j] = form_[i][j];
							beforeTurn_[k][l] = form_[k][l];*/

							if (form_[i][j] != Form::NONE && form_[k][l] != Form::NONE)
								//if (form_[i][j] != Form::GOAL && form_[k][l] != Form::GOAL)
							{
								//if(action_[i][j] == Action::Connect || action_[k][l] == Action::Connect)
								//�d�Ȃ��Ă���u���b�N������Œ�u���b�N��


								//��]������O�̏�Ԃ�ۑ�
								if (isTurn[i][j] == false || isTurn[k][l] == false)
								{
									beforeTurn_[i][j] = form_[i][j];
									beforeTurn_[k][l] = form_[k][l];

									//���o
									blocks_[i][j]->GetWorldTransForm()->scale =
									{ blocks_[i][j]->GetRadius() * 1.8f,blocks_[i][j]->GetRadius() * 1.8f, blocks_[i][j]->GetRadius() * 1.8f };
								}

								//��Ԃ�u���b�N��
								form_[i][j] = Form::LOCKED;
								form_[k][l] = Form::LOCKED;
								//�ω��t���O��I����
								isTurn[i][j] = true;
								isTurn[k][l] = true;
							}


						}

						//�`���[�g���A��
						if (tutorial->GetState() == TUTORIAL::OVERLAP)
						{
							//tutorial->AddStateNum();
						}
					}
				}
			}


		}
	}


}

//�d�Ȃ��Ă����u���b�N����ɖ߂�����
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

					//�d�Ȃ肪�O��Č��̏�Ԃɖ߂�����

					if (action_[i][j] == Action::Connect)
					{
						if (isOverlap == false)

							/*if (form_[i][j] == Form::LOCKED && form_[k][l] == Form::LOCKED &&
								action_[i][j] == Action::Connect )*/
						{
							//�o�O��₷���̂����I�I(�񂵂Ă���Ԃ̓v���C���[������Ă���{�^���͕ς��Ȃ��̂ŁA�{�^���Ƃ̔��肪�K�v)
							if (form_[i][j] == Form::LOCKED || form_[i][j] == Form::BUTTON && form_[k][l] == Form::LOCKED)
							{
								if (i != k || j != l)
								{
									//��ԕω�����ɖ߂�
									form_[i][j] = beforeTurn_[i][j];
									form_[k][l] = beforeTurn_[k][l];

									//�ω��t���O��I�t��
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

//�u���b�N�u���b�N�̋�`�̓����蔻��
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
			//�u���b�N�̎�ނ�ݒ�
			form_[i][j] = loadForms_[i][j];
			worldmats_[i][j] = loadWorldmats_[i][j];

			worldmats_[i][j].SetWorld();

			//���ɂȂ��Ă��邩�ǂ���
			isAxis_[i][j] = false;

			//���݂ǂ��Ȃ��Ă��邩
			action_[i][j] = Action::None;
		}
	}

	changedAction_ = false;
	isChanged_ = false;
	//��]

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
				//�p�[�e�B�N������
				ParticleManager::GetInstance()->GenerateRandomParticle(4, 100, 1.2f, worldmats_[i][j].trans, 0.4f, 0,
					{ 1.0f,1.0f,0.0f,1.0f }, { 1.0f,0.0f,0.0f,1.0f });
			}
		}
	}
}

void BlockManager::ChangePosY()
{
	for (int i = 0; i < stageWidth_; i++)
	{
		for (int j = 0; j < stageHeight_; j++)
		{
			if (action_[i][j] == Action::Connect)
			{
				if (form_[i][j] == Form::BLOCK && isUp[i][j] == false)
				{
					if (isLeftRolling == true || isRightRolling == true)
					{
						worldmats_[i][j].trans.y -= 0.01;

						isUp[i][j] = true;
					}
				}
			}
		}
	}
}

//�X�e�[�W��Z�b�g
void BlockManager::SetStage(const int& stageWidth, const int& stageHeight, std::vector<std::vector<WorldMat>>& worldmats, std::vector<std::vector<Form>>& forms)
{
	stageWidth_ = stageWidth;
	stageHeight_ = stageHeight;

	//�ǂݍ��񂾎��̍��W�ێ��p
	for (int i = 0; i < stageWidth_; i++)
	{
		//�u���b�N�^����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		loadWorldmats_.push_back(vector<WorldMat>());

		for (int j = 0; j < stageHeight_; j++)
		{
			//�u���b�N�̗v�f��ǉ�
			loadWorldmats_[i].push_back(worldmat_);
		}
	}

	//�ǂݍ��񂾌`�̍��W�ێ��p
	for (int i = 0; i < stageWidth_; i++)
	{
		//�u���b�N�^����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		loadForms_.push_back(vector<Form>());

		for (int j = 0; j < stageHeight_; j++)
		{
			//�u���b�N�̗v�f��ǉ�
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
	//�t�@�C����J��
	std::ifstream file;
	file.open("Resources\\blockPos.csv");
	assert(file.is_open());

	//�t�@�C���̓�e�𕶎���X�g���[���ɃR�s�[
	blocksPos << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void BlockManager::BlockPop(Vec3 pos)
{
	//�G�̐���

	//�x�N�^�z��ɗv�f<�u���b�N>��ǉ�
	for (int i = 0; i < stageWidth_; i++)
	{
		//�u���b�N�^����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		blocks_.push_back(vector<Block*>());

		for (int j = 0; j < stageHeight_; j++)
		{
			block_ = new Block;
			//�u���b�N�̗v�f��ǉ�
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
	//�ҋ@����
	if (isWaitBlock)
	{
		blockWaitTimer--;
		if (blockWaitTimer <= 0)
		{
			//�ҋ@����
			isWaitBlock = false;
		}
		return;
	}

	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(blocksPos, line))
	{
		//1�s���̕������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//,��؂�ōs�̐擪�������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0)
		{
			//�R�����g�s���΂�
			continue;
		}

		//POP�R�}���hcsv
		if (word.find("POP") == 0)
		{
			//X���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//Y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//Z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//�u���b�N�𔭐�������
			BlockPop(Vec3(x, y, z));
		}
		//WAIT�R�}���h
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			isWaitBlock = true;
			blockWaitTimer = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}
	}

}

