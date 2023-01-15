#include "BlockManager.h"

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
void BlockManager::Initialize(ConnectingEffectManager* connectEM, Camera* camera)
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

	//�x�N�^�z��ɗv�f<�u���b�N>��ǉ�
	for (int i = 0; i < blockWidth; i++)
	{
		//�u���b�N�^�����Ă��̃x�N�^��ǉ�(�s��ł���i��)
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
		//�u���b�N�^�����Ă��̃x�N�^��ǉ�(�s��ł���i��)
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


			blocks_[i][j]->Initialize(connectEM);

			//�u���b�N�̎�ނ�ݒ�
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


			//�u���b�N�̍��W��ݒ�
			if (i >= 0)
			{
				//blocks_[i][j]->GetWorldTransForm()->trans.x = i * (scale_.x * 1);
				worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);

			}
			if (j >= 0)
			{
				//blocks_[i][j]->GetWorldTransForm()->trans.y = i * (scale_.y * 1);

				worldmats_[i][j].trans.z = j * (worldmats_[i][j].scale.y * 2.0f);
			}

			worldmats_[i][j].SetWorld();

			block_->Initialize(connectEM);

			//���ɂȂ��Ă��邩�ǂ���
			isAxis_[i][j] = false;

			//���݂ǂ��Ȃ��Ă��邩
			action_[i][j] = Action::None;
		}
	}

	//���̑��̐ݒ�
	isCount = 1;
}

void BlockManager::Update()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//�u���b�N�̍��W��ݒ�
			if (i >= 0)
			{
				//blocks_[i][j]->GetWorldTransForm()->trans.x = i * (scale_.x * 1);
				//worldmats_[i][j].trans.x = i * (worldmats_[i][j].scale.x * 2.0f);

				//transforms[i][j].x += 0.001f;

				//�u���b�N�̍X�V
				//blocks_[i][j]->SetWorldPos(transforms[i][j]);
				//blocks_[i][j]->Updata(transforms[i][j]);
				/*worldmats_[i][j]->trans.x += 0.001f;
				worldmats_[i][j]->trans.z += 0.001f * j;*/

				//worldmats_[i][j]->SetWorld();

					//worldmats_[i][j]->SetWorld();
			}



			//X���W�̈�O�̔ԍ���ۑ�
			prevBlockY = j;

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

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			blocks_[i][j]->SetWorldPos(worldmats_[i][j].trans);
			//draw->DrawCube3D(worldmats_[i][j], &camera->viewMat, &camera->projectionMat);
			blocks_[i][j]->Draw(camera, texhandle, form_[i][j]);

			if (action_[i][j] == Action::Connect && effectCount >= effectCountMax)
			{
				if (isAxis_[i][j])
				{
					connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 35, 5, { 1.0f,0.3f,0.2f,1.0f });
				}
				else
				{
					connectEM->GenerateRandomConnectingEffect(worldmats_[i][j].trans, blockRadius_, blockRadius_ / 2.0f, 20, 3, { 1.0f,1.0f,0,1.0 });
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

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
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

//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)
bool BlockManager::GetPosIsBlock(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

			//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
			if (worldmats_[i][j].trans.x - blockRadius_ <= pos.x && worldmats_[i][j].trans.x + blockRadius_ >= pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ <= pos.z && worldmats_[i][j].trans.z + blockRadius_ >= pos.z)
			{
				//���̃u���b�N�̌`��͕��ʂ̃u���b�N���ǂ���
				if (form_[i][j] != Form::NONE)
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

	//player���ǂ̃u���b�N�ɂ����Ȃ�
	return false;
}

//�{�^�������邩�ǂ���
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
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
				else
				{
					return false;
				}
			}
		}
	}

	//�v���C���[���ǂ̃u���b�N�ɂ����Ȃ��ꍇ
	return false;

}

//�ŏ��Ɍq���{�^�����������u���b�N�����ɓo�^����֐�
void BlockManager::RegistAxisButton(const Vec3& pos)
{
	//�ŏ��Ƀ{�^�����������u���b�N�����ɓo�^����֐�
	//�����Ŏ󂯎�����v���C���[�̍��W�����ƂɌ��݈ʒu�̃{�^����ON�ɂ���

	//�v���C���[�̈ʒu�ɂ���u���b�N�����ɂ���


	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
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

					camera->CameraShake(10, 1.2f);
				}
				else {}
			}
			//�v���C���[���ǂ̃u���b�N�ɂ����Ȃ��ꍇ
			else {}
		}
	}

}

//�u���b�N���m���Ȃ��X�V�֐�
void BlockManager::UpdateConnect(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if ((worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
				&& action_[i][j] != Action::Connect)
			{
				action_[i][j] = Action::Connect;

				camera->CameraShake(10, 0.3f);

				////�q���͂���
				////�{�^�����q�����Ă��Ȃ���Ό��݈ʒu���Ȃ���Ԃɂ���
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
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//�v���C���[������u���b�N���ɂ�����
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

	//���݂̃u���b�N�����o�^����Ă���Ȃ�false
	return false;
}

//�q����Ă���u���b�N��S����������
void BlockManager::ReleseConectedBlock()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			if (action_[i][j] == Action::Connect)
			{
				//�S���������Ă��Ȃ���Ԃ�
				action_[i][j] = Action::None;
				isAxis_[i][j] = false;
			}
		}
	}
}

//�L�[�{�[�h�ɂ���ĉ�]

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


		//�p�x���K�v(�O�ɂ�����~�^�����Q�l�ɂȂ邩��)
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{

				//�����Ȃ����Ă���Ȃ�
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

		rotateCount++;
		if (rotateCount >= rotateCountMax)
		{
			isRightRolling = false;
			camera->CameraShake(10, 1.4f);
		}
	}

	if (isLeftRolling == true)
	{


		//�p�x���K�v(�O�ɂ�����~�^�����Q�l�ɂȂ邩��)
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//�����Ȃ����Ă���Ȃ�
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

		rotateCount++;
		if (rotateCount >= rotateCountMax)
		{
			isLeftRolling = false;
			camera->CameraShake(10, 1.4f);
		}
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
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if (worldmats_[i][j].trans.x - blockRadius_ < pos.x && worldmats_[i][j].trans.x + blockRadius_ > pos.x
				&& worldmats_[i][j].trans.z - blockRadius_ < pos.z && worldmats_[i][j].trans.z + blockRadius_ > pos.z)
			{
				//���̃u���b�N�̌`��̓{�^�����ǂ���
				if (form_[i][j] == Form::GOAL)
				{
					return true;
				}
			}
		}
	}

	//�v���C���[���ǂ̃u���b�N�ɂ����Ȃ��ꍇ
	return false;
}
