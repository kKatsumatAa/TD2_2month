#pragma once
#include<imgui.h>
#include"TextureManager.h"

//ImGui�̊Ǘ�
class ImGuiManager
{
private:


public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// imGui��t�J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// imgui��t�I��
	/// </summary>
	void End();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
};

