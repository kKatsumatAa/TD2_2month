#include "KeyboardInput.h"

KeyboardInput::KeyboardInput()
{
	result = DirectInput8Create(
		WindowsApp::GetInstance().Getw().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃��Z�b�g
	result = keyboard->SetCooperativeLevel(
		WindowsApp::GetInstance().Gethwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	//��ʂ���O�ɂ���Ƃ����͎�t�b�f�o�C�X�����̃A�v�������Ő�L���Ȃ��bWindows�L�[�𖳌�
	assert(SUCCEEDED(result));
}

KeyboardInput::~KeyboardInput()
{
}

KeyboardInput& KeyboardInput::GetInstance()
{
	static KeyboardInput inst; // private �ȃR���X�g���N�^���Ăяo���B
	return inst;
}

void KeyboardInput::Update()
{
	//�O��̃L�[���
	memcpy(oldkey, key, sizeof(key));

	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�S�L�[�̓��͏����擾
	keyboard->GetDeviceState(sizeof(key), key);
}
