#pragma once
class ConectLimit
{
public:

	//�Q�b�^�[
	int GetConectcount() { return conectCount; };
	//�Z�b�^�[
	void SetConectCount(int playConectCount) { conectCount -= playConectCount; };
	//���Z�b�g
	void ResetCount() { conectCount = limitCount; };

	//�c�ʐݒ�(�����Ŏc�ʂ�MAX������ݒ�)
	void SetCount(int setCountMax) { limitCount = setCountMax; };

private:

	//�q���񐔐����ۑ��p�̃J�E���g
	int limitCount;
	
	//�q����
	int conectCount;

};

