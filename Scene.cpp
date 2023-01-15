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

}

void SceneGame::Update()
{


	//シーン遷移
	/*if ()
	{
		scene->ChangeState(new SceneClear);
	}
	else if ()
	{
		scene->ChangeState(new SceneGameOver);
	}*/
}

void SceneGame::Draw()
{
}

void SceneGame::DrawSprite()
{
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
	/*if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		scene->ChangeState(new SceneLoad);
	}*/
}

void SceneClear::Draw()
{
}

void SceneClear::DrawSprite()
{
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
	imGuiManager->Finalize();
	delete imGuiManager;
	delete lightManager;
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

	model[0] = Model::LoadFromOBJ("sphere");

	//imgui
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize();

	blockManager = new BlockManager();
	blockManager->Initialize();

	//Light
	LightManager::StaticInitialize();
	//インスタンス生成
	lightManager = LightManager::Create();
	//ライト色を設定
	lightManager->SetDirLightColor(0, { 1,1,1 });
	//3Dオブジェクトにライトをセット(全体で一つを共有)
	Object::SetLight(lightManager);
	lightManager->SetDirLightActive(0, true);

	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialize();
	camera->SetEye({ 0, 20, -50 });
	camera->UpdateViewMatrix();

	//電気エフェクト
	connectEM = std::make_unique<ConnectingEffectManager>();
	connectEM->Initialize();

	//電気エフェクト
	player = std::make_unique<Player>();
	player->Initialize(5.0f, model[0], &debugText);

	//ステート変更
	ChangeState(new SceneLoad);
}

int count = 0;

void Scene::Update()
{
	//imgui
	imGuiManager->Begin();

	{
		//デモ
		ImGui::ShowDemoWindow();

	}

	blockManager->Update();
	state->Update();

	count++;
	if (/*KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE)*/count % 7 == 0)
	{
		connectEM->GenerateRandomConnectingEffect({ 0,0,0 }, 30.0f, 6.0f, 15, 20);
	}

	connectEM->Update();

	player->Update();


#ifdef _DEBUG
	//if (KeyboardInput::GetInstance().KeyTrigger(DIK_E)) ChangeState(new SceneTitle);
#endif 

	//imgui
	imGuiManager->End();
}

void Scene::Draw()
{
	state->Draw();
	blockManager->Draw(camera.get());

	player->Draw(camera.get());
	connectEM->Draw(*camera.get());

	blockManager->Draw(camera.get());
	
	//imgui
	imGuiManager->Draw();
}

void Scene::DrawSprite()
{
	state->DrawSprite();

	debugText.DrawAll(debugTextHandle);
}