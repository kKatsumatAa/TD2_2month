#pragma once

#include"BlockManager.h"
#include"Player.h"
#include"BlockManager.h"

class SaveData
{
public:
	Player player;
	PlayerSocket playerSocket;
	BlockManager blockManager;

public:
	SaveData(const SaveData& obj){};
	SaveData(){}
};

/// <summary>
/// ���߂邽�߂̏����������N���X
/// </summary>
class GetBackManager
{
private:
	GetBackManager(){}
	~GetBackManager();

	//���߂��Ƃ��ɂ��̃|�C���^�ɃZ�[�u�����������
	 Player* player_;
	 PlayerSocket* playerSocket_;
	 BlockManager* blockManager_;



	 SaveData saveData;

	 std::list<SaveData> saveDatas;

public:
	//�R�s�[�R���X�g���N�^�𖳌�
	GetBackManager(const GetBackManager& obj) = delete;
	//������Z�q��
	GetBackManager& operator=(const GetBackManager& obj) = delete;
	//�C���X�^���X���擾�i�V���O���g���j
	static GetBackManager* GetInstance();

	 void Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager);

	//���݂̏�Ԃ��Z�[�u
	 void SaveDatas();

	//���߂�
	 void GetBack();
};

