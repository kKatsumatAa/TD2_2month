#include "BlockManager.h"

//������
void BlockManager::Initialize()
{
	//�u���b�N�𐶐���������
	blocks_ = std::make_unique<std::vector <std::vector<Block>>>();
}

//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)
bool BlockManager::GetPosIsBlock(Vec3 pos)
{
	Vec3 playerPos = pos;

	

	return false;
}

//�{�^�������邩�ǂ���
bool BlockManager::GetPosIsButton(Vec3 pos)
{
	return false;
}

//�ŏ��Ɍq���{�^�����������u���b�N�����ɓo�^����֐�
void BlockManager::RegistButton(const Vec3& pos)
{
	//�ŏ��Ƀ{�^�����������u���b�N�����ɓo�^����֐�
	//�����Ŏ󂯎�����v���C���[�̍��W�����ƂɌ��݈ʒu�̃{�^����ON�ɂ���

	//�v���C���[�̈ʒu�ɂ���u���b�N�����ɂ���
	

}

//�u���b�N���m���Ȃ��X�V�֐�
void BlockManager::UpdateConnect(Vec3 pos)
{
}

//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
bool BlockManager::CheckAxisButton(Vec3 pos)
{


	return false;
}

//�q����Ă���u���b�N��S����������
void BlockManager::ReleseConectedBlock()
{
}

//�L�[�{�[�h�ɂ���ĉ�]
//�L�[�{�[�h���֐����Ŏ擾
void BlockManager::UpdateRotatePlayerBlock(Vec3& rotatePos)
{
}
