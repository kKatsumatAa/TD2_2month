#pragma once
#include"Collision.h"
#include"DebugText.h"
#include"ImGuiManager.h"
#include"ColliderManager.h"
#include"ParticleManager.h"
#include"Camera.h"
#include"Async.h"
#include"BlockManager.h"



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
	//��ԁi�s���j
	SceneState* state = nullptr;
	BlockManager* blockManager = nullptr;

public:
	WorldMat cameraWorldMat;
	const Vec3 cameraPos = { 0,0,-100 };

	DebugText debugText;

	//�f�o�b�O�e�L�X�g
	UINT64 debugTextHandle;
	UINT64 texhandle[10];

	//�摜�p�n���h��
	UINT64 textureHandle[30] = { 0 };

	//�����̉摜
	UINT64 textureNumHundle[12];

	//���f�[�^
	Sound::SoundData soundData[10];

	//imgui
	ImGuiManager* imGuiManager;

	//���C�g
	LightManager* lightManager = nullptr;

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1.0f,1.0f,1.0f };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };


	std::unique_ptr<Camera> camera;

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
