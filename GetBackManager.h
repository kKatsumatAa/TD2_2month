#pragma once

#include"BlockManager.h"
#include"Player.h"
#include"BlockManager.h"

class SaveData
{
public:
	std::unique_ptr < Player> player;
	std::unique_ptr < PlayerSocket> playerSocket;
	std::unique_ptr < BlockManager> blockManager;
	std::unique_ptr < CameraManager> cameraManager;
	std::unique_ptr < PredictBlockManager> predictBlockM;
	std::unique_ptr < RockOnImage> rockOnImage;

public:
	SaveData(const SaveData& obj) {};
	SaveData() { ; }
	~SaveData() { ; }
};

/// <summary>
/// ���߂邽�߂̏����������N���X
/// </summary>
class GetBackManager
{
private:
	GetBackManager() {}
	~GetBackManager();

	//���߂��Ƃ��ɂ��̃|�C���^�ɃZ�[�u�����������
	Player* player_;
	PlayerSocket* playerSocket_;
	BlockManager* blockManager_;
	CameraManager* cameraManager_;
	PredictBlockManager* predictBlockM;
	RockOnImage* rockOnImage;

	std::unique_ptr< SaveData> saveData;

	std::list < std::unique_ptr< SaveData>> saveDatas_;

public:
	//�R�s�[�R���X�g���N�^�𖳌�
	GetBackManager(const GetBackManager& obj) = delete;
	//������Z�q��
	GetBackManager& operator=(const GetBackManager& obj) = delete;
	//�C���X�^���X���擾�i�V���O���g���j
	static GetBackManager* GetInstance();

	void Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager, CameraManager* cameraM, PredictBlockManager* predictBlockM, RockOnImage* rockOnImage);

	//���݂̏�Ԃ��Z�[�u
	void SaveDatas();

	void Update();

	//���߂�
	void GetBack();
};

