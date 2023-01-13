#pragma once
#include "Block.h"
#include <vector>

class BlockManager
{

public:

	//������
	void Initialize();

	//�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)
	bool GetPosIsBlock(Vec3 pos);

	//�{�^�������邩�ǂ���
	bool GetPosIsButton(Vec3 pos);

	//�ŏ��Ɍq���{�^�����������u���b�N�����ɓo�^����֐�
	void RegistButton(const Vec3& pos);

	//�u���b�N���m���Ȃ��X�V�֐�
	void UpdateConnect(Vec3 pos);

	//�q���ۂɗ������Ƃ��낪���ȊO�̃{�^�����ǂ���
	bool CheckAxisButton(Vec3 pos);

	//�q����Ă���u���b�N��S����������
	void ReleseConectedBlock();

	//�L�[�{�[�h�ɂ���ĉ�]
	//�L�[�{�[�h���֐����Ŏ擾
	void UpdateRotatePlayerBlock(Vec3& rotatePos);


private:
	
	//�u���b�N�̓񎟌��z��
	std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;

};

