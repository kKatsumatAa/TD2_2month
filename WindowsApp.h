#pragma once
#include <Windows.h>
#include <d3d12.h>
#pragma comment(lib,"winmm.lib")


class WindowsApp final
{
private:
	WNDCLASSEX w;
	HWND hwnd;


	//�E�C���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	WindowsApp();

	~WindowsApp() { ; }


public:
	//�E�B���h�E�T�C�Y
	const float window_width = 1280.0f;
	const float window_height = 720.0f;

	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport;


	//�R�s�[�R���X�g���N�^�𖳌�
	WindowsApp(const WindowsApp& obj) = delete;
	//������Z�q��
	WindowsApp& operator=(const WindowsApp& obj) = delete;

	static WindowsApp& GetInstance();

	bool MessegeRoop(MSG msg);

	void UnregisterClassA()
	{
		UnregisterClass(w.lpszClassName, w.hInstance);
	}

	//void operator=(const WindowsApp& win)
	//{
	//	*this = win;
	//}

	//getter
	WNDCLASSEX Getw() { return w; }
	HWND Gethwnd() { return hwnd; }
};

