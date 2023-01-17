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
	scene->blockManager->Initialize(scene->connectEM.get(), scene->tutorial.get(), scene->camera.get(),
		scene->model[1], scene->model[2], scene->model[3], scene->model[4]);
	scene->camera->Initialize();
	scene->connectEM->Initialize();
	scene->player->Initialize(scene->blockManager->blockRadius_ * 2.0f, scene->blockManager, scene->playerSocket.get()
		, scene->connectE2M.get(), scene->tutorial.get(), scene->model[0], &scene->debugText);
	scene->playerSocket->Initialize(scene->model[0]);
}

void SceneGame::Update()
{
	scene->blockManager->Update();

	scene->connectEM->Update();
	scene->connectE2M->Update();

	scene->player->Update();

	Vec3 pos = scene->player->GetWorldPos();
	scene->playerSocket->Update({ pos.x,pos.y + scene->player->GetRadius(),pos.z });

	scene->tutorial->Update();

	//リセット
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_R))
	{
		scene->player->Reset();
		scene->blockManager->ResetBlock();
		scene->playerSocket->Initialize(scene->model[0]);
	}

	//シーン遷移
	if (scene->player->isGoal)
	{
		scene->ChangeState(new SceneClear);
	}
	/*
	else if ()
	{
		scene->ChangeState(new SceneGameOver);
	}*/
}

void SceneGame::Draw()
{
	scene->blockManager->Draw(scene->camera.get());

	scene->player->Draw(scene->camera.get());
	scene->playerSocket->Draw(scene->camera.get());
	scene->connectEM->Draw(*scene->camera.get());
	scene->connectE2M->Draw(scene->camera.get());
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
	scene->debugText.Print("clear", 10, 10, 114514, 5.0f);
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
	camera.reset();
	connectEM.reset();
	player.reset();
	playerSocket.reset();
	connectE2M.reset();
	tutorial.reset();
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
	TextureManager::LoadGraph(L"Resources/ascii.png", debugTextHandle);
	TextureManager::LoadGraph(L"Resources/image/effect1.png", texhandle[1]);

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

	//tutorial
	tutorial = std::make_unique<Tutorial>();
	tutorial->Initialize();

	//電気エフェクト
	connectEM = std::make_unique<ConnectingEffectManager>();
	connectEM->Initialize();

	blockManager = new BlockManager();
	blockManager->Initialize(connectEM.get(), tutorial.get(), camera.get(), model[1], model[2], model[3], model[4]);

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
	camera = std::make_unique<Camera>();
	camera->Initialize();
	camera->SetEye({ blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f, 40, -30 });
	camera->SetTarget({ blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f, 0, 0 });
	camera->UpdateViewMatrix();
	cameraPosImgui[0] = { blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f };
	cameraPosImgui[1] = { 35 };
	cameraPosImgui[2] = { -30 };
	cameraTarget[0] = { blockManager->blockWidth / 2.0f * blockManager->blockRadius_ * 2.0f };
	cameraTarget[1] = { 10 };
	cameraTarget[2] = { 0 };

	//playerSocket
	playerSocket = std::make_unique<PlayerSocket>();
	playerSocket->Initialize(model[0]);

	//effect2
	connectE2M = std::make_unique<ConnectingEffect2Manager>();
	connectE2M->Initialize();

	//player
	player = std::make_unique<Player>();
	player->Initialize(blockManager->blockRadius_ * 2.0f, blockManager, playerSocket.get(), connectE2M.get(), tutorial.get(), model[0], &debugText);



	//ステート変更
	ChangeState(new SceneGame);
}

int count = 0;

void Scene::Update()
{
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

		camera->SetEye({ cameraPosImgui[0],cameraPosImgui[1],cameraPosImgui[2] });
		camera->SetTarget({ cameraTarget[0],cameraTarget[1],cameraTarget[2] });


		ImGui::End();
		lightManager->Update();

	}

	//丸影
	lightManager->SetCircleShadowDir(0,
		XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2],0 }));
	lightManager->SetCircleShadowCasterPos(0,
		XMFLOAT3({ player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z }));
	lightManager->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightManager->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	lightManager->SetCircleShadowDistanceCasterLight(0, circleShadowDistance);

	camera->Update();

	blockManager->Update();

	state->Update();


#ifdef _DEBUG
	//if (KeyboardInput::GetInstance().KeyTrigger(DIK_E)) ChangeState(new SceneTitle);
#endif 

	//imgui
	imGuiManager->End();
}

void Scene::Draw()
{
	state->Draw();

	//imgui
	imGuiManager->Draw();
}

void Scene::DrawSprite()
{
	state->DrawSprite();

	debugText.DrawAll(debugTextHandle);
}
