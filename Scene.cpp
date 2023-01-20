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

}

void SceneTitle::Update()
{


	//シーン遷移
	/*if ()
	{
		scene->ChangeState(new SceneGame);
	}*/
}

void SceneTitle::Draw()
{
}

void SceneTitle::DrawSprite()
{
}


//---------------------------------------------------------------------------------------
//ゲーム
void SceneGame::Initialize()
{
	//カメラをゲームのメインカメラに
	scene->cameraM.get()->usingCamera = scene->cameraM->gameMainCamera.get();

	scene->blockManager->Initialize(scene->connectEM.get(), scene->tutorial.get(), scene->cameraM.get(),
		scene->goalE.get(), scene->model[1], scene->model[2], scene->model[3], scene->model[4]);
	scene->connectEM->Initialize();
	scene->player->Initialize(scene->blockManager->blockRadius_ * 2.0f, scene->blockManager, scene->playerSocket.get()
		, scene->connectE2M.get(), scene->tutorial.get(), scene->cameraM.get(), scene->model[0], &scene->debugText);
	scene->playerSocket->Initialize(scene->connectE2M.get(), scene->blockManager->blockRadius_, scene->model[0]);
	scene->tutorial->Initialize();
	scene->goalE->Initialize(scene->cameraM.get());
	scene->stageManager->Initialize(scene->blockManager);
}

void SceneGame::Update()
{
	//if (scene->player->isGoal)
	//{
	//	scene->cameraM->usingCamera = scene->cameraM->goalEffectCamera.get();
	//}
	//else
	//{
	//	scene->cameraM->usingCamera = scene->cameraM->goalEffectCamera.get();
	//}

	if (!scene->player->isGoal) {
		

		scene->connectEM->Update();
		scene->connectE2M->Update();

		scene->player->Update();
		scene->blockManager->Update();

		Vec3 pos = scene->player->GetWorldPos();
		scene->playerSocket->Update({ pos.x,pos.y + scene->player->GetRadius(),pos.z });

		scene->tutorial->Update();

		//リセット
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
		{
			//カメラをゲームのメインカメラに
			scene->cameraM.get()->usingCamera = scene->cameraM->gameMainCamera.get();
			scene->player->Reset();
			scene->blockManager->ResetBlock();
			scene->connectE2M->Initialize();
			scene->playerSocket->Initialize(scene->connectE2M.get(), scene->blockManager->blockRadius_, scene->model[0]);
			scene->tutorial->Initialize();
		}
	}
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
	/*
	else if ()
	{
		scene->ChangeState(new SceneGameOver);
	}*/
}

void SceneGame::Draw()
{
	scene->blockManager->Draw(scene->cameraM.get()->usingCamera);

	scene->player->Draw(scene->cameraM.get()->usingCamera);
	scene->playerSocket->Draw(scene->cameraM.get()->usingCamera);
	scene->connectEM->Draw(*scene->cameraM.get()->usingCamera);
	scene->connectE2M->Draw(scene->cameraM.get()->usingCamera);

	ParticleManager::GetInstance()->Draw(scene->texhandle[1]);
}

void SceneGame::DrawSprite()
{
	scene->tutorial->Draw();
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
}

void SceneClear::Update()
{


	//シーン遷移
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		scene->ChangeState(new SceneGame);
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
void SceneLoad::Initialize()
{

	//非同期処理(ステージ作成中にもロード画面出す的な)
	//async.StartAsyncFunction([=]() { /*ここに関数 */ });
}

void SceneLoad::Update()
{


	//シーン遷移
	//if (async.GetLockFlag())
	//{
	//	async.EndThread();

	//	//ステージ作り終わったら
	//	scene->ChangeState(new SceneTitle);
	//}
}

void SceneLoad::Draw()
{
}

void SceneLoad::DrawSprite()
{
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
	imGuiManager->Finalize();
	delete imGuiManager;
	delete lightManager;

	delete model[0];
	delete model[1];
	delete model[2];
	delete model[3];
	delete model[4];

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
	//白い画像
	TextureManager::GetInstance().LoadGraph(L"Resources/image/white.png", TextureManager::GetInstance().whiteTexHandle);

	//画像
	{
		TextureManager::LoadGraph(L"Resources/ascii.png", debugTextHandle);
		TextureManager::LoadGraph(L"Resources/image/effect1.png", texhandle[1]);
		//クリア
		TextureManager::LoadGraph(L"Resources/image/Temp_GameClearScene.png", texhandle[2]);
	}

	//model
	Model::StaticInitialize();

	model[0] = Model::LoadFromOBJ("Player");
	model[1] = Model::LoadFromOBJ("Mesh_NormalTile_01");
	model[2] = Model::LoadFromOBJ("Mesh_ButtonTile_01");
	model[3] = Model::LoadFromOBJ("Mesh_GoalTile_01");
	model[4] = Model::LoadFromOBJ("Mesh_SocketTile_01");

	//imgui
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize();

	//パーティクル
	ParticleManager::GetInstance()->Initialize();

	//tutorial
	tutorial = std::make_unique<Tutorial>();
	tutorial->Initialize();
	
	//goal
	goalE = std::make_unique<GoalEffect>();
	goalE->Initialize(cameraM.get());

	//電気エフェクト
	connectEM = std::make_unique<ConnectingEffectManager>();
	connectEM->Initialize();

	
	blockManager = new BlockManager();
	blockManager->Initialize(connectEM.get(), tutorial.get(), cameraM.get(), goalE.get(), model[1], model[2], model[3], model[4]);

	stageManager = std::make_unique<StageManager>();
	stageManager->Initialize(blockManager);
	stageManager->SetTutorial(13,13);


	//Light
	LightManager::StaticInitialize();
	//インスタンス生成
	lightManager = LightManager::Create();
	//ライト色を設定
	lightManager->SetDirLightColor(0, { 1,1,1 });
	//3Dオブジェクトにライトをセット(全体で一つを共有)
	Object::SetLight(lightManager);
	lightManager->SetDirLightActive(0, true);
	lightManager->SetDirLightActive(1, false);
	lightManager->SetDirLightActive(2, false);
	//点光源
	for (int i = 0; i < 6; i++)
	{
		lightManager->SetPointLightActive(i, false);
	}
	//丸影
	lightManager->SetCircleShadowActive(0, true);


	//カメラ
	cameraPosImgui[0] = { blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f };
	cameraPosImgui[1] = { 35 };
	cameraPosImgui[2] = { -30 };
	cameraTarget[0] = { blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f };
	cameraTarget[1] = { 10 };
	cameraTarget[2] = { 0 };
	

	cameraM = std::make_unique<CameraManager>();
	//ゲームのメインカメラをセット
	cameraM->usingCamera = cameraM->gameMainCamera.get();
	//cameraM->gameMainCamera->SetEye({ blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f, 40, -30 });
	//cameraM->gameMainCamera->SetTarget({ blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f, 0, 0 });
	cameraM->usingCamera->SetEye({ cameraPosImgui[0],cameraPosImgui[1],cameraPosImgui[2] });
	cameraM->usingCamera->SetTarget({ cameraTarget[0],cameraTarget[1],cameraTarget[2] });
	cameraM->gameMainCamera->UpdateViewMatrix();


	//playerSocket
	playerSocket = std::make_unique<PlayerSocket>();
	playerSocket->Initialize(connectE2M.get(), blockManager->blockRadius_, model[0]);

	//effect2
	connectE2M = std::make_unique<ConnectingEffect2Manager>();
	connectE2M->Initialize();

	//player
	player = std::make_unique<Player>();
	player->Initialize(blockManager->blockRadius_ * 2.0f, blockManager, playerSocket.get(), connectE2M.get(), tutorial.get(), cameraM.get(),model[0], &debugText);



	//ステート変更
	ChangeState(new SceneGame);
}

int count = 0;

void Scene::Update()
{
#ifdef _DEBUG

	//imgui
	imGuiManager->Begin();

	{
		//デモ
		ImGui::ShowDemoWindow();



		static bool a = true;
		ImGui::Begin("circleShadow", &a, ImGuiWindowFlags_MenuBar);

		ImGui::InputFloat3("cameraPos", cameraPosImgui);
		ImGui::InputFloat3("cameraTarget", cameraTarget);
		/*ImGui::InputFloat2("circleShadowFactorAngle", circleShadowFactorAngle);
		ImGui::InputFloat("distanceLight", &circleShadowDistance);*/

		ImGui::End();
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

	blockManager->Update();

	if (KeyboardInput::GetInstance().KeyPush(DIK_0))
	{
		stageManager->SetTutorial(13, 13);

	}
	else if (KeyboardInput::GetInstance().KeyPush(DIK_1))
	{
		stageManager->SetStage1(13, 13);
	}
	else if (KeyboardInput::GetInstance().KeyPush(DIK_2))
	{
		stageManager->SetStage2(2, 10);
	}
	

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
