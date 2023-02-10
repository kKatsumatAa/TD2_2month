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

void GetBackManager::Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager, CameraManager* cameraM, PredictBlockManager* predictBlockM)
{
	saveDatas_.clear();

	player_ = player;
	playerSocket_ = playerSocket;
	blockManager_ = blockManager;
	cameraManager_ = cameraM;
	this->predictBlockM = predictBlockM;
	//SaveDatas();
}

void GetBackManager::SaveDatas()
{
	saveData = std::make_unique<SaveData>();

	saveData->player = std::make_unique<Player>();
	saveData->playerSocket = std::make_unique<PlayerSocket>();
	saveData->blockManager = std::make_unique<BlockManager>();
	saveData->cameraManager = std::make_unique<CameraManager>();
	saveData->predictBlockM = std::make_unique<PredictBlockManager>();

	//中身のみコピー(ポインターを渡しても同じものなので)
	saveData->player->operator=(*player_);
	saveData->playerSocket->operator=(*playerSocket_);
	saveData->blockManager->operator=(*blockManager_);
	saveData->cameraManager->operator=(*cameraManager_);
	saveData->predictBlockM->operator=(*predictBlockM);

	saveDatas_.push_back(std::move(saveData));
}

void GetBackManager::Update()
{
	return;
}



void GetBackManager::GetBack()
{
	if (saveDatas_.size() > 0)
	{
		//end()は最終要素の一個後ろなので itr--;
		std::list< std::unique_ptr< SaveData>>::iterator itr = saveDatas_.end();
		itr--;

		player_->operator=(std::move(*(*itr).get()->player.get()));
		playerSocket_->operator=(std::move (*(*itr).get()->playerSocket.get()));
		blockManager_->operator=(std::move(*(*itr).get()->blockManager.get()));
		cameraManager_->operator=(std::move(*(*itr).get()->cameraManager.get()));
		predictBlockM->operator=(std::move(*(*itr).get()->predictBlockM.get()));

		saveDatas_.erase(itr);

		//delete (*itr);
	}
}


//-----------------------------