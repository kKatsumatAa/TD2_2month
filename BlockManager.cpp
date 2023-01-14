#include "BlockManager.h"

using namespace std;

BlockManager::~BlockManager()
{

	//�u���b�N�̍폜
	//blocks_.clear();
}

//������
void BlockManager::Initialize()
{

	//std::unique_ptr<Block> newBullet = std::make_unique<Block>();

	//Block* block_ = nullptr;
	//block_ = new Block;

	//������
	block_.Initialize();
	
	//�x�N�^�z��ɗv�f<�u���b�N>��ǉ�
	for (int i = 0; i < blockWidth; i++)
	{
		//�u���b�N�^�����Ă��̃x�N�^��ǉ�(�s��ł���i��)
		blocks_.push_back(vector<Block>());
		
		for (int j = 0; j < blockHeight; j++)
		{
			//�u���b�N�̗v�f��ǉ�
			blocks_[i].push_back(block_);
		}
	}

	
	//�u���b�N�̑傫��
	
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			
			blocks_[i][j].Initialize();

			//�u���b�N�̎�ނ�ݒ�
			if (i == 1 && j == 1)
			{
				form_[i][j] = Form::BUTTON;
			}
			else
			{
				form_[i][j] = Form::BLOCK;
			}

			//�u���b�N�̍��W��ݒ�
			if (i > 0)
			{
				worldTransform_[i][j].trans.x += i * (scale_.x * 2);
			}
			if (j > 0)
			{
				worldTransform_[i][j].trans.z += j * (scale_.z * 2);
			}

			//�u���b�N�̊e�t���O��ݒ�
			//isRotate_[i][j] = false;
			////�d�Ȃ��Ă��邩�ǂ���
			//isOverlap_[i][j] = false;
			////�q�����Ă��邩�ǂ���
			//isConnect_[i][j] = false;
			////���ɂȂ��Ă��邩�ǂ���
			//isAxis_[i][j] = false;

			//���݂ǂ��Ȃ��Ă��邩
			action_[i][j] = Action::None;
		}
	}

	//���̑��̐ݒ�
	isCount = 1;
	scale_ = { 3.0,3.0,3.0 };

}

void BlockManager::Update()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//���
			//preWorldTransform_[i][j] = worldTransform_[i][j];

			//�u���b�N�̍X�V
			blocks_[i][j].Updata();

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
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			blocks_[i][j].Draw(camera);
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
			//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
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

//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)
bool BlockManager::GetPosIsBlock(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//�v���C���[���u���b�N�̏�ɂ��邩�ǂ���
			if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
				&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
			{
				//���̃u���b�N�̌`��͕��ʂ̃u���b�N���ǂ���
				if (form_[i][j] == Form::BLOCK)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			//�v���C���[���ǂ̃u���b�N�ɂ����Ȃ��ꍇ
			else
			{
				return false;
			}
		}
	}
}

//�{�^�������邩�ǂ���
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
			if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
				&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
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
			//�v���C���[���ǂ̃u���b�N�ɂ����Ȃ��ꍇ
			else
			{
				return false;
			}
		}
	}
}

//�ŏ��Ɍq���{�^�����������u���b�N�����ɓo�^����֐�
void BlockManager::RegistAxisButton(const Vec3& pos, bool isConnect)
{
	//�ŏ��Ƀ{�^�����������u���b�N�����ɓo�^����֐�
	//�����Ŏ󂯎�����v���C���[�̍��W�����ƂɌ��݈ʒu�̃{�^����ON�ɂ���

	//�v���C���[�̈ʒu�ɂ���u���b�N�����ɂ���

	if (isConnect == true)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
				if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
					&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
				{
					//���̃u���b�N�̌`��̓{�^�����ǂ���
					if (form_[i][j] == Form::BUTTON && isAxis_[i][j] == false)
					{
						//���o�^����
						isAxis_[i][j] = true;
					}
					else{}
				}
				//�v���C���[���ǂ̃u���b�N�ɂ����Ȃ��ꍇ
				else{}
			}
		}
	}

}

//�u���b�N���m���Ȃ��X�V�֐�
void BlockManager::UpdateConnect(Vec3 pos, bool isConnect)
{
	if (isConnect == true)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//�v���C���[���w��̃u���b�N�̏�ɂ��邩�ǂ���
				if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
					&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
				{
					//�q���͂���
					//�{�^�����q�����Ă��Ȃ���Ό��݈ʒu���Ȃ���Ԃɂ���
					if (form_[i][j] == Form::BUTTON && action_[i][j] == Action::None)
					{
						action_[i][j] = Action::Connect;
					}

					//�q������(�E�����e�X�g)
					if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHT) && i < blockWidth - 1 && changedAction_ == true && isChanged_ == true)
					{
						//�O�u���b�N���q�����Ă���Όq����
						if (action_[i][j] == Action::Connect)
						{
							if (form_[i + 1][j] == Form::BLOCK)
							{
								//�u���b�N�Ȃ�q����
								action_[i + 1][j] = Action::Connect;
								isChanged_ = false;
							}
							else if (form_[i + 1][j] == Form::GEAR)
							{
								//�M�A�Ȃ�q���Ď~�߂�
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

//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
bool BlockManager::CheckAxisButton(Vec3 pos, bool isConnect)
{
	if (isConnect == true)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//�v���C���[������u���b�N���ɂ�����
				if (worldTransform_[i][j].trans.x - radius_ < pos.x && worldTransform_[i][j].trans.x + radius_ > pos.x
					&& worldTransform_[i][j].trans.z - radius_ < pos.z && worldTransform_[i][j].trans.z + radius_ > pos.z)
				{
					//���݂̃u���b�N�����o�^����Ă���Ȃ�false
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
			}
		}
	}
}

//�L�[�{�[�h�ɂ���ĉ�]
//�L�[�{�[�h���֐����Ŏ擾
void BlockManager::UpdateRotatePlayerBlock(Vec3& rotatePos, bool isRota)
{
	//���Ɍ����Ă̕����x�N�g�����擾

	//�p�x���K�v(�O�ɂ�����~�^�����Q�l�ɂȂ邩��)

	if (isRota)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//�����Ȃ����Ă���Ȃ�
				if (action_[i][j] == Action::Connect)
				{

				}

				//��]����
			}
		}
	}
}
