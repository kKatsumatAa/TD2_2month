#pragma once
#include"StageManager.h"


class Hint;

//�X�e�[�g�e�N���X
class HintState
{
protected:
	Hint* hint = nullptr;
	//
	Object object;

	//�^�C�}�[�n
	int timer = 0;
	const int timerMax = 20;
	float t = 0;

public:
	void SetHint(Hint* hint);
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

//�\������ĂȂ��@�X�e�[�g�q�N���X
class HintStateNormal : public HintState
{
private:

public:
	void Update()override;
	void Draw()override;
};

//�\�����Ă���@�X�e�[�g�q�N���X
class HintStateDisplay : public HintState
{
private:


public:
	void Update()override;
	void Draw()override;
};


class Hint final
{
	//�ϐ�
private:
	UINT64 texHandle[StageManager::stageMax];

	//�\������
	bool isDisplayingHint = false;

	int stageNum = 0;

	//�X�e�[�g
	HintState* state = nullptr;

public:



	//�֐�
private:
	Hint() { ; }
	~Hint() { ; }
public:
	//�R�s�[�R���X�g���N�^�𖳌�
	Hint(const Hint& obj) = delete;
	//������Z�q��
	Hint& operator=(const Hint& obj) = delete;

	static Hint& GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	//���݂̃X�e�[�W���w��
	void SetStageNum(const int& num) { this->stageNum = num; }

	//�q���g��\�������ǂ���
	bool GetIsDisplayingHint() { return isDisplayingHint; }
	//
	int GetStageNum() { return stageNum; }

	//�X�e�[�g�ύX
	void ChangeState(HintState* state);

	UINT64* GetTexHandle() { return texHandle; }
};

