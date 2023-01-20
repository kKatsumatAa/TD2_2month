#pragma once
#include"Collision.h"
#include"DebugText.h"
#include"ImGuiManager.h"
#include"ColliderManager.h"
#include"ParticleManager.h"
#include"Camera.h"
#include"Async.h"
#include"ConnectingEffect.h"
#include"Player.h"
#include"BlockManager.h"
#include"StageManager.h"
#include"CameraManager.h"




class Scene;

class SceneState
{
protected:
	Scene* scene;

public:
	virtual void Initialize() = 0;
	void SetScene(Scene* scene);
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawSprite() = 0;
};

class Scene
{
private:
	//状態（行動）
	SceneState* state = nullptr;


public:
	WorldMat cameraWorldMat;
	const Vec3 cameraPos = { 0,0,-100 };

	Model* model[10];

	DebugText debugText;

	//デバッグテキスト
	UINT64 debugTextHandle;
	UINT64 texhandle[10];

	//画像用ハンドル
	UINT64 textureHandle[30] = { 0 };

	//数字の画像
	UINT64 textureNumHundle[12];

	//音データ
	Sound::SoundData soundData[10];

	//imgui
	ImGuiManager* imGuiManager;

	//ライト
	LightManager* lightManager = nullptr;

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.15f,0 };
	float circleShadowFactorAngle[2] = { 0,0.7f };
	float circleShadowDistance = 100.0f;

	//camera
	float cameraPosImgui[3] = { 0 };
	float cameraTarget[3] = { 0 };


	std::unique_ptr<CameraManager> cameraM;

	std::unique_ptr<ConnectingEffectManager> connectEM;

	std::unique_ptr<Player> player;
	std::unique_ptr<PlayerSocket> playerSocket;
	std::unique_ptr<ConnectingEffect2Manager> connectE2M;
	std::unique_ptr<GoalEffect> goalE;

	std::unique_ptr<Tutorial> tutorial;

	BlockManager* blockManager = nullptr;
	
	std::unique_ptr<StageManager> stageManager= nullptr;

public:
	~Scene();
	void ChangeState(SceneState* state);

	void Initialize();
	void Update();
	void Draw();
	void DrawSprite();
	
};

class SceneTitle : public SceneState
{
private:


public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;
};

class SceneGame : public SceneState
{
private:


public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;
};

class SceneGameOver : public SceneState
{
private:


public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawSprite()override;
};

class SceneClear : public SceneState
{
private:
	int count = 0;
	const int countMax = 40;
	Object sprite;

public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawSprite()override;
};

class SceneLoad : public SceneState
{
private:
	int count = 0;
	Async async;

public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawSprite()override;
};
