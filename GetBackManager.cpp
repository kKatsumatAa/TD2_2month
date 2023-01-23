#include "GetBackManager.h"

GetBackManager::~GetBackManager()
{
	saveDatas_.clear();
}

GetBackManager* GetBackManager::GetInstance()
{
	static GetBackManager instance;
	return &instance;
}

void GetBackManager::Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager, CameraManager* cameraM)
{
	saveDatas_.clear();

	player_ = player;
	playerSocket_ = playerSocket;
	blockManager_ = blockManager;
	cameraManager_ = cameraM;

	SaveDatas();
}

void GetBackManager::SaveDatas()
{
	saveData = new SaveData;

	//�V���ɏꏊ���m�ۂ��Ă����ɃR�s�[
	saveData->player = new Player();
	saveData->playerSocket = new PlayerSocket;
	saveData->blockManager = new BlockManager;
	saveData->cameraManager = new CameraManager;
	//���g�̂݃R�s�[(�|�C���^�[��n���Ă��������̂Ȃ̂�)
	saveData->player->operator=(*player_);
	*saveData->playerSocket = *playerSocket_;
	saveData->blockManager->operator=(*blockManager_);
	saveData->cameraManager->operator=(*cameraManager_);

	saveDatas_.push_back(saveData);
}

void GetBackManager::Update()
{
	return;
}



void GetBackManager::GetBack()
{
	if (saveDatas_.size() > 0)
	{
		std::list<SaveData*>::iterator itr = saveDatas_.end();
		itr--;

		player_->operator=(*(**itr).player);
		*playerSocket_ = *(**itr).playerSocket;
		blockManager_->operator=(*(**itr).blockManager);
		cameraManager_->operator=(*(**itr).cameraManager);

		saveDatas_.pop_back();
	}
}
