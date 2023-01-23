#pragma once
#include"Camera.h"

class CameraManager;

class UsingCameraState
{
protected:
	CameraManager* cameraM = nullptr;

public:
	void SetCameraM(CameraManager* cameraM);
	virtual void Update() = 0;
};

class UsingCameraNormalState : public UsingCameraState
{
private:


public:
	void Update() override;
};

class UsingCameraLerpMoveState : public UsingCameraState
{
private:

public:
	void Update() override;
};


class CameraManager
{
private:
	UsingCameraState* state = nullptr;

public:
	//�����ɂ����ȃJ�����̃|�C���^�����āA�`�掞�Ɏg��
	Camera* usingCamera;
	//�I����ʂŎg��
	std::unique_ptr<Camera> stageSelectCamera;
	//�Q�[�������C���Ŏg���J����
	std::unique_ptr<Camera> gameMainCamera;
	//�Q�[�����A��]���Ɏg���J����
	std::unique_ptr<Camera> gameTurnCamera;
	//�S�[�����o���Ɏg���J����
	std::unique_ptr<Camera> goalEffectCamera;

	//���o�Ȃǂ��I�������ɂ��̃J�����ɐ؂�ւ��邽�߂̃|�C���^
	Camera* afterCamera = nullptr;
	//�I�������A�J�����ɐ؂�ւ���܂ł̃t���[����
	int afterCount = 0;

	//lerp�p
	int lerpCount = 0;
	int lerpCountMax = 0;
	Vec3 startEye, endEye, startTarget, endTarget, startUp, endUp;

	bool isLerpMoving = false;

public://�֐�
	CameraManager();
	~CameraManager();

	void ChangeUsingCameraState(UsingCameraState* state);

	void Initialize();

	void Update();

	//�ړ��̐��`�⊮���n�߂�
	void BegineLerpUsingCamera(Vec3 startEye, Vec3 endEye, Vec3 startTarget, Vec3 endTarget, Vec3 startUp, Vec3 endUp, int time, Camera* afterCamera = nullptr, int afterCount = 0);


	CameraManager& operator=(const CameraManager& obj);
};