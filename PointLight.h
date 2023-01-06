#pragma once
#include<DirectXMath.h>
#include <wrl.h>
using namespace Microsoft::WRL;

/// <summary>
/// �_����
/// </summary>
class PointLight
{
private://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

private://�����o�ϐ�
	//���C�g���W�i���[���h���W�n�j
	XMFLOAT3 lightpos = { 0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1.0f,1.0f,1.0f };
	//���C�g���������W��
	XMFLOAT3 lightatten = { 1.0f,1.0f,1.0f };
	//�L���t���O
	bool active = false;



public://�����o�֐�
	inline void SetLightPos(const XMFLOAT3& lightpos) { this->lightpos = lightpos; }
	inline const XMFLOAT3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const XMFLOAT3& lightatten) { this->lightatten = lightatten; }
	inline const XMFLOAT3& GetLightAtten() { return lightatten; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool GetActive() { return active; }
};

