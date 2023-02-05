#pragma once
#include"TextureManager.h"
#include"Util.h"
#include"Camera.h"
#include"DrawIntNumImage.h"

class ConectLimit
{
public:
	void Initialize();

	void Update(Vec3 pos, Camera* camera);

	void Draw();


	//�Q�b�^�[
	int GetConectcount() { return conectCount; };
	int GetLimitCount() { return limitCount; };
	//�Z�b�^�[
	void SetConectCount(int playConectCount) { conectCount -= playConectCount; }
	//���Z�b�g
	void ResetCount() { conectCount = limitCount; };

	//���Z�b�g
	void ResetCounts() { conectCount = 0; limitCount = 0; }

	//�c�ʐݒ�(�����Ŏc�ʂ�MAX������ݒ�)
	void SetCount(int setCountMax) { limitCount = setCountMax; };

	void SetDrawCount(int count) { drawConnectCount = count; }

private:
	UINT64 texhandle;

	DrawIntNumImage drawNum;

	//�q���񐔐����ۑ��p�̃J�E���g
	int limitCount;

	//�q����
	int conectCount;

	//�\���p
	int drawConnectCount;
};

