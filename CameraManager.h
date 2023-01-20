#pragma once
#include"Camera.h"

class CameraManager
{
private:


public:
	//�����ɂ����ȃJ�����̃|�C���^�����āA�`�掞�Ɏg��
	Camera* usingCamera;
	//�Q�[�������C���Ŏg���J����
	std::unique_ptr<Camera> gameMainCamera;
	//�Q�[�����A��]���Ɏg���J����
	std::unique_ptr<Camera> gameTurnCamera;
	//�S�[�����o���Ɏg���J����
	std::unique_ptr<Camera> goalEffectCamera;


public://�֐�
	CameraManager();
	~CameraManager();

	void Initialize();

	void Update();

};