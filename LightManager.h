#pragma once
#include "DirectX.h"
#include "PointLight.h"
#include "DirLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"


class LightManager
{
public:
	//�_�����̐�
	static const int PointLightNum = 6;

private://�G�C���A�X
//Microsoft::WRL::���ȗ�
	template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private://�萔

	//���s����
	static const int DirLightNum = 3;

	//�X�|�b�g���C�g�̐�
	static const int SpotLightNum = 3;

	//�ۉe�̐�
	static const int CircleShadowNum = 1;

private://�ÓI�����o�ϐ�
	static ID3D12Device* device_;

private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�_�[�e�B�t���O
	bool dirty = false;
	// �����̐F
	XMFLOAT3 ambientColor = { 1,1,1 };
	//�_�����̔z��
	PointLight pointLights[PointLightNum];
	// ���s�����̔z��
	DirLight dirLights[DirLightNum];
	//�X�|�b�g���C�g�̔z��
	SpotLight spotLights[SpotLightNum];
	//�ۉe�̔z��
	CircleShadow circleShadows[CircleShadowNum];

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���s�����A�_�����̔z����܂Ƃ߂đ���)
	struct ConstBufferData
	{
		// �����̐F
		XMFLOAT3 ambientColor;
		float pad1;

		// ���s�����̔z��
		DirLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//�ۉe�p
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};


private:
	/// <summary>
/// �萔�o�b�t�@�]��
/// </summary>
	void TransferConstBuffer();

public://�ÓI�����o�֐�
	static void StaticInitialize();

public:
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��i�`��͂����A�萔�o�b�t�@�r���[�̃Z�b�g�̂ݍs���j
	/// </summary>
	/// <param name="rootParamaterIndex"></param>
	void Draw(UINT rootParamaterIndex);

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static LightManager* Create();

	/// <summary>
		/// ���s�����̗L���t���O���Z�b�g
		/// </summary>
		/// <param name="index">���C�g�ԍ�</param>
		/// <param name="active">�L���t���O</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

///�|�C���g���C�g�p
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3& pos);
	void SetPointLightColor(int index, const XMFLOAT3& color);
	void SetPointLightAtten(int index, const XMFLOAT3& atten);

///�X�|�b�g���C�g�p
	void SetSpotLightActive(int index, bool active);
	//���C�g�̕������Z�b�g
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);
	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);
	//�����W�����Z�b�g
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);
	//�����J�n�A�I���p�x
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);

///�ۉe�p
	void SetCircleShadowActive(int index, bool active);
	//�e�̔������̃I�u�W�F�N�g�̍��W
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);
	//���C�g�̕������Z�b�g
	void SetCircleShadowDir(int index, const XMVECTOR& lightdir);
	//�e���烉�C�g�܂ł̋���
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	//�����W�����Z�b�g
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);
	//�����J�n�A�I���p�x
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle);
};

