#include "GetBackManager.h"

GetBackManager::~GetBackManager()
{
	saveDatas.clear();
}

GetBackManager* GetBackManager::GetInstance()
{
	static GetBackManager instance;
	return &instance;
}

void GetBackManager::Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager)
{
	saveDatas.clear();

	player_ = player;
	playerSocket_ = playerSocket;
	blockManager_ = blockManager;

	saveData.player.operator=(*player);
	saveData.playerSocket = *playerSocket;
	saveData.blockManager.operator=(* blockManager);

	//‰Šú‚Ìó‘Ô‚ğ“o˜^
	saveDatas.push_back(saveData);
}

void GetBackManager::SaveDatas()
{
	saveData.player.operator=(*player_);
	saveData.playerSocket = *playerSocket_;
	saveData.blockManager.operator=(*blockManager_);

	saveDatas.push_back(saveData);
}



void GetBackManager::GetBack()
{
	if (saveDatas.size())
	{
		player_->operator=(saveDatas.end()->player);
		*playerSocket_ = saveDatas.end()->playerSocket;
		blockManager_->operator=(saveDatas.end()->blockManager);

		saveDatas.erase(saveDatas.end());
	}
}
