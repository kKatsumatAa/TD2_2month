#include "Scene.h"
#include <sstream>
#include <iomanip>

void SceneState::SetScene(Scene* scene)
{
	//state�ł͂Ȃ�Scene�N���X�̃C���X�^���X//
	this->scene = scene;
}



//---------------------------------------------------------------------------------------
//�^�C�g��
void SceneTitle::Initialize()
{

}

void SceneTitle::Update()
{


	//�V�[���J��
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
//�Q�[��
void SceneGame::Initialize()
{

}

void SceneGame::Update()
{


	//�V�[���J��
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
//�I�����
void SceneGameOver::Initialize()
{
}

void SceneGameOver::Update()
{


	//�V�[���J��
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


	//�V�[���J��
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

	//�񓯊�����(�X�e�[�W�쐬���ɂ����[�h��ʏo���I��)
	//async.StartAsyncFunction([=]() { /*�����Ɋ֐� */ });
}

void SceneLoad::Update()
{


	//�V�[���J��
	//if (async.GetLockFlag())
	//{
	//	async.EndThread();

	//	//�X�e�[�W���I�������
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
//�f�X�g���N�^
Scene::~Scene()
{
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
	//�����摜
	TextureManager::GetInstance().LoadGraph(L"Resources/image/white.png", TextureManager::GetInstance().whiteTexHandle);

	//�摜
	TextureManager::LoadGraph(L"Resources/ascii.png", debugTextHandle);
	TextureManager::LoadGraph(L"Resources/image/effect1.png", texhandle[1]);

	//model
	Model::StaticInitialize();

	model[0] = Model::LoadFromOBJ("sphere");

	//imgui
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize();

	//Light
	LightManager::StaticInitialize();
	//�C���X�^���X����
	lightManager = LightManager::Create();
	//���C�g�F��ݒ�
	lightManager->SetDirLightColor(0, { 1,1,1 });
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g(�S�̂ň�����L)
	Object::SetLight(lightManager);
	lightManager->SetDirLightActive(0, true);

	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialize();

	//�d�C�G�t�F�N�g
	connectEM = std::make_unique<ConnectingEffectManager>();
	connectEM->Initialize();

	//�d�C�G�t�F�N�g
	player = std::make_unique<Player>();
	player->Initialize(5.0f, model[0], &debugText);

	//�X�e�[�g�ύX
	ChangeState(new SceneLoad);
}

int count = 0;

void Scene::Update()
{
	//imgui
	imGuiManager->Begin();

	{
		//�f��
		ImGui::ShowDemoWindow();

	}

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

	player->Draw(camera.get());
	connectEM->Draw(*camera.get());

	//imgui
	imGuiManager->Draw();
}

void Scene::DrawSprite()
{
	state->DrawSprite();

	debugText.DrawAll(debugTextHandle);
}