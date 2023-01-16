#pragma once

#include<DirectXMath.h>

/// <summary>
/// �X�|�b�g���C�g
/// </summary>
class SpotLight
{
private://�G�C���A�X
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
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

public://�����o�֐�

	inline void SetLightDir(const XMVECTOR& lightdir) {
		this->lightdir = DirectX::XMVector3Normalize(lightdir);
	};
	inline const XMVECTOR& GetLightDir() {
		return lightdir;
	};
	inline void SetLightPos(const XMFLOAT3& lightpos) {
		this->lightpos = lightpos;
	};
	inline const XMFLOAT3& GetLightPos() {
		return lightpos;
	};
	inline void SetLightColor(const XMFLOAT3& lightcolor) {
		this->lightcolor = lightcolor;
	};
	inline const XMFLOAT3& GetLightColor() {
		return lightcolor;
	};
	inline void SetLightAtten(const XMFLOAT3& lightAtten) {
		this->lightAtten = lightAtten;
	};
	inline const XMFLOAT3& GetLightAtten() {
		return lightAtten;
	};
	inline void SetLightFactorAngleCos(const XMFLOAT2& lightFactoeAngleCos) {
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactoeAngleCos.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactoeAngleCos.y));
	};
	inline const XMFLOAT2& GetLightFactorAngleCos() {
		return lightFactorAngleCos;
	};
	inline void SetLightActive(bool active) {
		this->active = active;
	};
	inline const bool GetLightActive() {
		return active;
	};


private://�����o�ϐ�
	//���C�g����
	XMVECTOR lightdir = { 1.0f,0,0 };
	//���C�g���W�i���[���h���W�n�j
	XMFLOAT3 lightpos = { 0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//���C�g���������W��
	XMFLOAT3 lightAtten = { 1.0f,1.0f,1.0f };
	//���C�g�����p�x�i�J�n�p�x�A�I���p�x�j
	XMFLOAT2 lightFactorAngleCos = { 0.5f,0.2f };
	//�L���t���O
	bool active = false;

};

