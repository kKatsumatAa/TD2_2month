#include "Scene.h"
#include <sstream>
#include <iomanip>

void SceneState::SetScene(Scene* scene)
{
	//stateではなくSceneクラスのインスタンス//
	this->scene = scene;
}

//---------------------------------------------------------------------------------------
//タイトル
void SceneTitle::Initialize()
{
	scene->lightManager->SetCircleShadowActive(0, false);
	//スペースキー
	obj[0].worldMat->scale = { 310 / 15.0f,50 / 15.0f,1.0f };
	obj[0].worldMat->trans = { 0,-33.0f,0.0f };
	obj[0].worldMat->SetWorld();
	//背景
	obj[1].worldMat->scale = { WindowsApp::GetInstance().window_width / 17.5f,WindowsApp::GetInstance().window_height / 17.5f ,1.0f };
	obj[1].worldMat->trans = { 0,0,0.1f };
	obj[1].worldMat->SetWorld();

	scene->StopAllWave();
	//音
	Sound::GetInstance().PlayWave("titleBGM.wav", 0.5f, true);
}

void SceneTitle::Update()
{
	if (count % 30 == 0)
	{
		float x = (float)rand() / RAND_MAX * 200.0f - 100.0f;
		ParticleManager::GetInstance()->GenerateRandomParticle(20, 120, 3.0f, { x,50,0 }, 3.0f, 0.1f, { 0.1f,0.2f,0.9f,0.9f }, { 0,0,0,0.7f });
	}

	ParticleManager::GetInstance()->Update(&scene->cameraM->usingCamera->viewMat, &scene->cameraM->usingCamera->projectionMat);

	//シーン遷移
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		Sound::GetInstance().PlayWave("select.wav", 0.7f);

		scene->ChangeState(new SceneStageSelect);
	}
}

void SceneTitle::Draw()
{
	count++;

	obj[0].worldMat->trans = { 0,-33.0f + sinf(count * 0.025f) * 1.5f,0.0f };
	obj[0].worldMat->SetWorld();

	//背景
	if (alpha < 1.0f)
	{
		alpha += 0.01f;
	}
	obj[1].DrawBox(obj[1].worldMat, &scene->cameraM->usingCamera->viewMat, &scene->cameraM->usingCamera->projectionMat,
		{ 1.0f,1.0f,1.0f,1.0f }, scene->texhandle[9]);

	//スペースキー
	obj[0].DrawBox(obj[0].worldMat, &scene->cameraM->usingCamera->viewMat, &scene->cameraM->usingCamera->projectionMat,
		{ 1.0f,1.0f,1.0f,1.0f }, scene->texhandle[8]);

	ParticleManager::GetInstance()->Draw(scene->texhandle[1]);
}

void SceneTitle::DrawSprite()
{

}

//---------------------------------------------------------------------------------------
//セレクト画面
void SceneStageSelect::Initialize()
{
	//Object::effectFlags.isBarrelCurve = true;

	scene->lightManager->SetCircleShadowActive(0, false);
	scene->stageSelectM->Initialize(scene->stageManager.get());
	scene->cameraM->Initialize();
	scene->tutorial->Initialize();

	//音
	scene->StopAllWave();
	Sound::GetInstance().PlayWave("LevelSelect.wav", 0.5f, true);
}

void SceneStageSelect::Update()
{
	scene->stageSelectM->Update();

	ParticleManager::GetInstance()->Update(&scene->cameraM.get()->usingCamera->viewMat, &scene->cameraM.get()->usingCamera->projectionMat);

	//選択されたら
	if (scene->stageSelectM->isSelect)
	{
		ParticleManager::GetInstance()->GenerateRandomParticle(50, 120, 3.0f, { 0,0,0 }, 5.0f, 0.1f, { 0.1f,0.5f,0.5,0.99f }, { 0,0,0,0 });
		scene->ChangeState(new SceneLoad);
	}
	//ステージセレクトに戻る
	else if (KeyboardInput::GetInstance().KeyTrigger(DIK_Q) || KeyboardInput::GetInstance().KeyTrigger(DIK_ESCAPE))
	{
		//scene->cameraM->usingCamera = scene->cameraM->stageSelectCamera.get();
		scene->ChangeState(new SceneTitle);
	}
}

void SceneStageSelect::Draw()
{
	scene->stageSelectM->Draw(scene->cameraM.get());

	ParticleManager::GetInstance()->Draw(scene->texhandle[1]);
}

void SceneStageSelect::DrawSprite()
{
	scene->stageSelectM->DrawSprite();

	obj[3].DrawBoxSprite({ 50,50,0 }, 0.2f, { 1.0f,1.0f,1.0f,1.0f }, scene->texhandle[7]);
}


//---------------------------------------------------------------------------------------
//ゲーム
void SceneGame::Initialize()
{

}

void SceneGame::Update()
{
	if (!scene->player->isGoal) {

		scene->blockManager->Update();
		//特定のカメラ演出時は動かさない
		if (!scene->blockManager->isPopedGoal2)
		{
			scene->player->Update();


			Vec3 pos = scene->player->GetWorldPos();
			scene->playerSocket->Update({ pos.x,pos.y,pos.z });


			scene->predictBlockManager->Update();

			scene->rockOnImage->Update();
		}
		if (scene->stageSelectM->isTutorial)
		{
			scene->tutorial->Update();
		}
		//リセット
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_R) || scene->player->PlayerOutArea())
		{
			//カメラをゲームのメインカメラに
			scene->cameraM.get()->usingCamera = scene->cameraM->gameMainCamera.get();
			scene->cameraM.get()->Initialize();
			scene->player->Reset();
			scene->blockManager->ResetBlock();
			scene->connectE2M->Initialize();
			scene->predictBlockManager->Initialize();
			scene->playerSocket->Initialize(scene->connectE2M.get(), scene->blockManager->blockRadius_, scene->model[0]);
			scene->rockOnImage->Initialize();
			if (scene->stageManager->selectStage == STAGE::TUTORIAL)
			{
				scene->tutorial->Initialize();
			}
			if (scene->stageManager->selectStage == STAGE::STAGE1)
			{
				scene->tutorial->ConnectLimitInitialize();
			}
			if (scene->stageManager->selectStage == STAGE::STAGE2)
			{
				scene->tutorial->OverlapInitialize();
			}
			if (scene->stageManager->selectStage == STAGE::STAGE3)
			{
				scene->tutorial->ElectricInitialize();
			}
			if (scene->stageManager->selectStage == STAGE::STAGE5)
			{
				scene->tutorial->ButtonInitialize();
			}

			GetBackManager::GetInstance()->Initialize(scene->player.get(), scene->playerSocket.get(), scene->blockManager, scene->cameraM.get());
		}
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_Z))
		{
			GetBackManager::GetInstance()->GetBack();
		}
	}
	scene->goalConnectEM->Update();
	scene->connectEM->Update();
	scene->connectE2M->Update();
	ParticleManager::GetInstance()->Update(&scene->cameraM.get()->usingCamera->viewMat, &scene->cameraM.get()->usingCamera->projectionMat);
	//シーン遷移
	if (scene->player->isGoal)
	{
		scene->goalE->Update();

		if (scene->goalE->isEnd)
		{
			scene->ChangeState(new SceneClear);
		}
	}
	//ステージセレクトに戻る
	else if (KeyboardInput::GetInstance().KeyTrigger(DIK_Q) || KeyboardInput::GetInstance().KeyTrigger(DIK_ESCAPE))
	{
		scene->cameraM->usingCamera = scene->cameraM->stageSelectCamera.get();
		scene->ChangeState(new SceneStageSelect);
	}
}

void SceneGame::Draw()
{
	scene->objWallFloor[1].DrawModel(scene->objWallFloor[1].worldMat, &scene->cameraM.get()->usingCamera->viewMat, &scene->cameraM.get()->usingCamera->projectionMat,
		scene->model[12], { 0.7f,0.7f,0.7f,1.0f });

	scene->objWallFloor[0].DrawModel(scene->objWallFloor[0].worldMat, &scene->cameraM.get()->usingCamera->viewMat, &scene->cameraM.get()->usingCamera->projectionMat,
		scene->model[7], { 0.7f,0.7f,0.7f,1.0f });

	scene->blockManager->Draw(scene->cameraM.get()->usingCamera);

	if (!scene->blockManager->isPopedGoal2)
	{
		scene->player->Draw(scene->cameraM.get()->usingCamera);
	}
	scene->playerSocket->Draw(scene->cameraM.get()->usingCamera);
	scene->connectEM->Draw(*scene->cameraM.get()->usingCamera);
	scene->connectE2M->Draw(scene->cameraM.get()->usingCamera);

	scene->predictBlockManager->Draw(scene->cameraM.get()->usingCamera, !scene->blockManager->isPopedGoal2);

	scene->goalConnectEM->Draw(scene->cameraM.get()->usingCamera);

	ParticleManager::GetInstance()->Draw(scene->texhandle[1]);

	scene->conectLimit_->Update(scene->player->GetWorldPos(), scene->cameraM->usingCamera);
}

void SceneGame::DrawSprite()
{
	if (scene->stageSelectM->isTutorial)
	{
		scene->tutorial->Draw();
	}

	obj[3].DrawBoxSprite({ 50,50,0 }, 0.2f, { 1.0f,1.0f,1.0f,1.0f }, scene->texhandle[7]);
	obj[0].DrawBoxSprite({ 160,50,0 }, 0.2f, { 1.0f,1.0f,1.0f,1.0f }, scene->texhandle[3]);
	obj[1].DrawBoxSprite({ 280,50,0 }, 0.2f, { 1.0f,1.0f,1.0f,1.0f }, scene->texhandle[4]);

	obj[2].DrawBoxSprite({ 0,0,0 }, 1.0f, { 1.0f,1.0f,1.0f,0.7f }, scene->texhandle[6]);

	if (!scene->player->isGoal)
	{
		scene->conectLimit_->Draw();
		scene->rockOnImage->Draw(scene->cameraM.get()->usingCamera);
	}
}



//---------------------------------------------------------------------------------------
//終了画面
void SceneGameOver::Initialize()
{
}

void SceneGameOver::Update()
{


	//シーン遷移
	/*if ()
	{
		scene->ChangeState(new SceneLoad);
	}*/
}

void SceneGameOver::Draw()
{
}

void SceneGameOver::DrawSprite()
{

}


//----------------------------------------------------------------------------------------
void SceneClear::Initialize()
{
	//音
	scene->StopAllWave();
	Sound::GetInstance().PlayWave("game-victory-sound-effect.wav", 0.5f);
}

void SceneClear::Update()
{
	//シーン遷移
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		//カメラ切り替え
		scene->cameraM->usingCamera = scene->cameraM->stageSelectCamera.get();

		scene->ChangeState(new SceneStageSelect);
	}
}

void SceneClear::Draw()
{
}

void SceneClear::DrawSprite()
{
	count++;

	float alpha = (float)count / (float)countMax;

	sprite.DrawBoxSprite({ 0,0,0 }, 1.0f, { 1.0f,1.0f,1.0f,alpha }, scene->texhandle[2]);
}


//--------------------------------------------------------------------------------------
void SceneLoad::LoadFunc()
{
	scene->rockOnImage->Initialize();

	scene->goalConnectEM->Initialize();

	//Object::effectFlags.isScanningLine = false;

	scene->predictBlockManager->Initialize();

	//ステージ
	scene->objWallFloor[0].worldMat->trans = { {scene->stageManager->stageWidth / 2.0f * scene->blockManager->blockRadius_ * 2.0f }
	,-scene->blockManager->blockRadius_ / 2.0f ,0 - scene->blockManager->blockRadius_ };
	scene->objWallFloor[0].worldMat->scale = { scene->blockManager->blockRadius_ ,scene->blockManager->blockRadius_ ,scene->blockManager->blockRadius_ };
	scene->objWallFloor[0].worldMat->SetWorld();

	scene->objWallFloor[1].worldMat->trans = { {scene->stageManager->stageWidth / 2.0f * scene->blockManager->blockRadius_ * 2.0f }
	,0 - scene->blockManager->blockRadius_ * 1.0f ,0 - scene->blockManager->blockRadius_ };
	//scene->objWallFloor[1].worldMat->scale = { 0.5f,0.5f,0.5f };
	scene->objWallFloor[1].worldMat->SetWorld();

	//丸影
	scene->lightManager->SetCircleShadowActive(0, true);



	scene->blockManager->Initialize(scene->rockOnImage, scene->connectEM.get(), scene->predictBlockManager.get(), scene->tutorial.get(), scene->cameraM.get(),
		scene->goalE.get(), scene->goalConnectEM.get(),
		scene->model[1], scene->model[2], scene->model[3], scene->model[4], scene->model[5], scene->model[6],
		scene->model[8], scene->model[9], scene->model[10], scene->model[11], scene->model[13], scene->model[14]);
	scene->connectEM->Initialize();
	scene->connectE2M->Initialize();

	scene->playerSocket->Initialize(scene->connectE2M.get(), scene->blockManager->blockRadius_, scene->model[0]);
	scene->goalE->Initialize(scene->cameraM.get());

	scene->player->Initialize(scene->blockManager->blockRadius_ * 2.0f, scene->blockManager, scene->playerSocket.get()
		, scene->connectE2M.get(), scene->tutorial.get(), scene->cameraM.get(), scene->model[0], &scene->debugText, scene->conectLimit_);
	scene->player->SetPosStage(scene->stageManager->playerPos);
	scene->stageManager->Initialize(scene->blockManager, scene->tutorial.get(), scene->conectLimit_);
	scene->player->SetConectCount(scene->stageManager->GetConectCount());

	GetBackManager::GetInstance()->Initialize(scene->player.get(), scene->playerSocket.get(), scene->blockManager, scene->cameraM.get());

	//カメラ位置セット
	scene->cameraM->gameMainCamera->SetEye({ { scene->stageManager->stageWidth / 2.0f * scene->blockManager->blockRadius_ * 2.0f }
	,42,-30 });
	scene->cameraM->gameMainCamera->SetTarget({ scene->stageManager->stageWidth / 2.0f * scene->blockManager->blockRadius_ * 2.0f
		,10
		,0 });
	scene->cameraM->gameMainCamera->UpdateViewMatrix();

	//カメラをゲームのメインカメラに
	scene->cameraM.get()->usingCamera = scene->cameraM->gameMainCamera.get();
	scene->cameraM->Initialize();

	//音
	scene->StopAllWave();
	Sound::GetInstance().PlayWave("Stage_BGM.wav", 0.5f, true);
}

void SceneLoad::Initialize()
{

	//非同期処理(ステージ作成中にもロード画面出す的な)
	async.StartAsyncFunction([=]() { LoadFunc(); });
}

void SceneLoad::Update()
{
	if (count % 5 == 0)
	{
		float x = (float)rand() / RAND_MAX * 200.0f - 100.0f;
		ParticleManager::GetInstance()->GenerateRandomParticle(20, 120, 3.0f, { x,50,0 }, 3.0f, 0.1f, { 1.0f,1.0f,0.2f,0.7f }, { 0,0,0,0 });
	}

	ParticleManager::GetInstance()->Update(&scene->cameraM->usingCamera->viewMat, &scene->cameraM->usingCamera->projectionMat);

	//シーン遷移
	if (async.GetLockFlag())
	{
		async.EndThread();

		//ステージ作り終わったら
		ParticleManager::GetInstance()->ClearParticles();

		scene->ChangeState(new SceneGame);
	}
}

void SceneLoad::Draw()
{
	ParticleManager::GetInstance()->Draw(scene->texhandle[1]);
}

void SceneLoad::DrawSprite()
{
	count++;

	sprite[0].DrawBoxSprite({ 0,0,0 }, 1.0f, { 1.0f,1.0f,1.0f,(float)count / (float)countMax }, scene->texhandle[5]);
}





//---------------------------------------------------------------------------------------
//デストラクタ
Scene::~Scene()
{
	delete blockManager;
	cameraM.reset();
	connectEM.reset();
	player.reset();
	playerSocket.reset();
	connectE2M.reset();
	tutorial.reset();
	goalE.reset();
	stageSelectM.reset();
	stageManager.reset();
	goalConnectEM.reset();
	imGuiManager->Finalize();
	delete imGuiManager;
	delete lightManager;
	delete rockOnImage;

	delete model[0];
	delete model[1];
	delete model[2];
	delete model[3];
	delete model[4];
	delete model[5];
	delete model[6];
	delete model[7];
	delete model[8];
	delete model[9];
	delete model[10];
	delete model[11];
	delete model[12];
	delete model[13];
	delete model[14];
}

void Scene::ChangeState(SceneState* state)
{
	if (state) {
		delete this->state;
	}
	this->state = state;
	state->SetScene(this);
	this->state->Initialize();
}

void Scene::Initialize()
{
	//音
	{
		Sound::GetInstance().Initialize("Resources/Sounds/");
		Sound::GetInstance().LoadWave("titleBGM.wav", false);
		Sound::GetInstance().LoadWave("Stage_BGM.wav", false);
		Sound::GetInstance().LoadWave("LevelSelect.wav", false);
		Sound::GetInstance().LoadWave("game-victory-sound-effect.wav", false);

		Sound::GetInstance().LoadWave("arrow (2).wav", false);
		Sound::GetInstance().LoadWave("button (2).wav", false);
		Sound::GetInstance().LoadWave("connectBegine.wav", false);
		Sound::GetInstance().LoadWave("connectEnd.wav", false);
		Sound::GetInstance().LoadWave("connectCancel.wav", false);
		Sound::GetInstance().LoadWave("connectGoal.wav", false);
		Sound::GetInstance().LoadWave("connectMove.wav", false);
		Sound::GetInstance().LoadWave("emergeGoal.wav", false);
		Sound::GetInstance().LoadWave("move (2).wav", false);
		Sound::GetInstance().LoadWave("moveFailed (2).wav", false);
		Sound::GetInstance().LoadWave("select.wav", false);
		Sound::GetInstance().LoadWave("turnBegine.wav", false);
		Sound::GetInstance().LoadWave("turnEnd.wav", false);
		Sound::GetInstance().LoadWave("limitFailed.wav", false);
		Sound::GetInstance().LoadWave("goal.wav", false);
		Sound::GetInstance().LoadWave("tutu4.wav", false);


	}

	//白い画像
	TextureManager::GetInstance().LoadGraph(L"Resources/image/white.png", TextureManager::GetInstance().whiteTexHandle);

	//画像
	{
		TextureManager::LoadGraph(L"Resources/ascii.png", debugTextHandle);
		TextureManager::LoadGraph(L"Resources/image/effect1.png", texhandle[1]);
		//クリア
		TextureManager::LoadGraph(L"Resources/image/clear.png", texhandle[2]);
		//ゲーム中のリセット,一手戻る
		TextureManager::LoadGraph(L"Resources/image/z.png", texhandle[3]);
		TextureManager::LoadGraph(L"Resources/image/restart.png", texhandle[4]);
		//ロード
		TextureManager::LoadGraph(L"Resources/image/LoadingScreen.png", texhandle[5]);
		//枠
		TextureManager::LoadGraph(L"Resources/image/UI/UI_Border.png", texhandle[6]);
		//Q
		TextureManager::LoadGraph(L"Resources/image/backStageQ.png", texhandle[7]);
		//タイトル
		TextureManager::GetInstance().LoadGraph(L"Resources/image/title.png", texhandle[9]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/press_space.png", texhandle[8]);
	}

	//model
	Model::StaticInitialize();

	model[0] = Model::LoadFromOBJ("Player");
	model[1] = Model::LoadFromOBJ("Mesh_NormalTile_01");
	model[2] = Model::LoadFromOBJ("Mesh_ButtonTile_01");//固定ブロックに変える
	model[3] = Model::LoadFromOBJ("Goal_02");
	model[4] = Model::LoadFromOBJ("Mesh_SocketTile_01");
	model[5] = Model::LoadFromOBJ("Button");
	model[6] = Model::LoadFromOBJ("DisconnectedBlock");

	model[7] = Model::LoadFromOBJ("Floor");
	model[8] = Model::LoadFromOBJ("DisconnectedButton");
	model[9] = Model::LoadFromOBJ("DisconnectedSocketBlock");
	model[10] = Model::LoadFromOBJ("ElectricBlock");
	model[11] = Model::LoadFromOBJ("DoorGoal_Closed");
	model[12] = Model::LoadFromOBJ("Wall");
	model[13] = Model::LoadFromOBJ("OverlapBlock");
	model[14] = Model::LoadFromOBJ("BeforeButtonPop");

	//imgui
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize();

	conectLimit_ = new ConectLimit();
	conectLimit_->Initialize();

	/*conectLimit_ = std::make_unique<ConectLimit>();
	conectLimit_->ResetCounts();
	*/
	//予測線用
	predictBlockManager = std::make_unique<PredictBlockManager>();
	predictBlockManager->Initialize();

	//パーティクル
	ParticleManager::GetInstance()->Initialize();

	//
	rockOnImage = new RockOnImage();
	rockOnImage->Initialize();

	//tutorial
	tutorial = std::make_unique<Tutorial>();

	//goal
	goalE = std::make_unique<GoalEffect>();
	goalE->Initialize(cameraM.get());

	//電気エフェクト
	connectEM = std::make_unique<ConnectingEffectManager>();
	connectEM->Initialize();

	//電気エフェクト
	goalConnectEM = std::make_unique<GoalConnectEffectManager>();
	goalConnectEM->Initialize();


	blockManager = new BlockManager();
	blockManager->Initialize(rockOnImage, connectEM.get(), predictBlockManager.get(), tutorial.get(), cameraM.get(), goalE.get(), goalConnectEM.get(),
		model[1], model[2], model[3], model[4], model[5], model[6], model[8], model[9], model[10], model[11], model[13], model[14]);

	stageManager = std::make_unique<StageManager>();
	stageManager->Initialize(blockManager, tutorial.get(), conectLimit_);


	//Light
	LightManager::StaticInitialize();
	//インスタンス生成
	lightManager = LightManager::Create();
	//ライト色を設定
	lightManager->SetDirLightColor(0, { 1,1,1 });
	//3Dオブジェクトにライトをセット(全体で一つを共有)
	Object::SetLight(lightManager);
	lightManager->SetDirLightActive(0, true);
	lightManager->SetDirLightActive(1, true);
	lightManager->SetDirLightActive(2, false);
	lightManager->SetDirLightDir(0, XMVectorSet(0, 0, 1.0f, 0));
	lightManager->SetDirLightDir(1, XMVectorSet(0, -1.0f, 0, 0));
	//点光源
	for (int i = 0; i < 6; i++)
	{
		lightManager->SetPointLightActive(i, false);
	}
	//丸影
	lightManager->SetCircleShadowActive(0, true);

	cameraM = std::make_unique<CameraManager>();
	cameraM->Initialize();
	//ステージ選択用のカメラ
	cameraM->usingCamera = cameraM->stageSelectCamera.get();
	//cameraM->gameMainCamera->SetEye({ blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f, 40, -30 });
	//cameraM->gameMainCamera->SetTarget({ blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f, 0, 0 });



	//playerSocket
	playerSocket = std::make_unique<PlayerSocket>();
	playerSocket->Initialize(connectE2M.get(), blockManager->blockRadius_, model[0]);

	//effect2
	connectE2M = std::make_unique<ConnectingEffect2Manager>();
	connectE2M->Initialize();

	//player
	player = std::make_unique<Player>();
	player->Initialize(blockManager->blockRadius_ * 2.0f, blockManager, playerSocket.get(), connectE2M.get(), tutorial.get(), cameraM.get(), model[0], &debugText, conectLimit_);

	//ステージセレクトマネージャー
	stageSelectM = std::make_unique<StageSelectManager>();
	stageSelectM->Initialize(stageManager.get());



	//ステート変更
	ChangeState(new SceneTitle);
}

int count = 0;

void Scene::Update()
{
#ifdef _DEBUG

	//imgui
	imGuiManager->Begin();

	//{
	//	//デモ
	//	ImGui::ShowDemoWindow();

	//	static bool a = true;
	//	ImGui::Begin("circleShadow", &a, ImGuiWindowFlags_MenuBar);

	//	ImGui::InputFloat3("cameraPos", cameraPosImgui);
	//	ImGui::InputFloat3("cameraTarget", cameraTarget);
	//	/*ImGui::InputFloat2("circleShadowFactorAngle", circleShadowFactorAngle);
	//	ImGui::InputFloat("distanceLight", &circleShadowDistance);*/

	//	ImGui::End();
	//}

	//ブロックウインドウの表示
	{
		//blockManager->SetElec(elec);

		//int count = player->GetPlayerConectCount();
		//int countMax = player->GetPlayerConectCountMax();


		//ImGui::Begin("conectCount");
		////ImGui::SetWindowPos("Elec", ImVec2(100, 100));
		//ImGui::SetWindowSize("conectCount", ImVec2(400, 100));
		////ImGui::Text("制限数");
		//ImGui::InputInt("conectCount", &count, 0.0f);
		////ImGui::Text("最大制限");
		//ImGui::InputInt("conectCountMax", &countMax, 0.0f);


		//ImGui::End();
	}

	//ブロックウインドウの表示
	{

		//blockManager->SetElec(elec);

		//ImGui::Begin("Elec");
		////ImGui::SetWindowPos("Elec", ImVec2(100, 100));
		//ImGui::SetWindowSize("Elec", ImVec2(600, 800));

		//for (int i = 0; i < 13; i++)
		//{
		//	ImGui::Text("%d", i);
		//	ImGui::InputInt13("isElec", elec[i], 0.0f);
		//}

		//ImGui::End();
	}

	//imgui
	imGuiManager->End();
#endif 


	//丸影
	lightManager->SetCircleShadowDir(0,
		XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2],0 }));
	lightManager->SetCircleShadowCasterPos(0,
		XMFLOAT3({ player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z }));
	lightManager->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightManager->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	lightManager->SetCircleShadowDistanceCasterLight(0, circleShadowDistance);
	lightManager->Update();

	cameraM->Update();

	state->Update();

#ifdef _DEBUG
	//if (KeyboardInput::GetInstance().KeyTrigger(DIK_E)) ChangeState(new SceneTitle);
#endif 


}

void Scene::Draw()
{
	state->Draw();
}

void Scene::DrawSprite()
{
	state->DrawSprite();

#ifdef _DEBUG
	debugText.DrawAll(debugTextHandle);

	//imgui
	imGuiManager->Draw();
#endif 
}

void Scene::DrawPostEffect()
{
	Object::DrawPera();
}

void Scene::StopAllWave()
{
	Sound::GetInstance().StopWave("LevelSelect.wav");
	Sound::GetInstance().StopWave("Stage_BGM.wav");
	Sound::GetInstance().StopWave("titleBGM.wav");

	Sound::GetInstance().StopWave("arrow (2).wav");
	Sound::GetInstance().StopWave("button (2).wav");
	Sound::GetInstance().StopWave("connectBegine.wav");
	Sound::GetInstance().StopWave("connectEnd.wav");
	Sound::GetInstance().StopWave("connectCancel.wav");
	Sound::GetInstance().StopWave("connectGoal.wav");
	Sound::GetInstance().StopWave("connectMove.wav");
	Sound::GetInstance().StopWave("emergeGoal.wav");
	Sound::GetInstance().StopWave("move (2).wav");
	Sound::GetInstance().StopWave("moveFailed (2).wav");
	//Sound::GetInstance().StopWave("select.wav");
	Sound::GetInstance().StopWave("turnBegine.wav");
	Sound::GetInstance().StopWave("turnEnd.wav");
	Sound::GetInstance().StopWave("limitFailed.wav");
	Sound::GetInstance().StopWave("goal.wav");
	Sound::GetInstance().StopWave("tutu4.wav");
}


