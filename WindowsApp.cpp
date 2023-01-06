#include "WindowsApp.h"
#include<imgui_impl_win32.h>


// imgui��p�̃E�B���h�E�v���V�[�W��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam);


LRESULT WindowsApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	
	//imgui�p�̃E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}
	
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏C����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsApp::WindowsApp()
{
	//�V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

	//�E�C���h�E�N���X�̐ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		//�E�C���h�E�v���V�[�W���̐ݒ�
	w.lpszClassName = L"DirectX";				//�E�C���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);		//�E�C���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	//�E�C���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	//�E�C���h�E�T�C�Y{X,Y,����,�c��}
	RECT wrc = { 0,0,(LONG)window_width,(LONG)window_height };
	//�����ŃT�C�Y�␳
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(
		w.lpszClassName,		//�N���X��
		L"LE2A_04_�C�C�__���E�L",			//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	//�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,			//�\��X���W(OS�ɔC����)
		CW_USEDEFAULT,			//�\��Y���W(OS�ɔC����)
		wrc.right - wrc.left,	//�E�B���h�E����
		wrc.bottom - wrc.top,	//�E�B���h�E�c��
		nullptr,				//�e�E�B���h�E�n���h��
		nullptr,				//���j���[�n���h��
		w.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr					//�I�v�V����
	);

	//�E�C���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);

	viewport = { 0, 0, window_width, window_height, 0.0f, 1.0f };
}

WindowsApp& WindowsApp::GetInstance()
{
	static WindowsApp inst; // private �ȃR���X�g���N�^���Ăяo���B
	return inst;
}

bool WindowsApp::MessegeRoop(MSG msg)
{
	//���b�Z�[�W�����邩?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI��
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}
