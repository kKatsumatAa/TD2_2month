#pragma once
#include "DirectX.h"
#include "PointLight.h"
#include "Light.h"

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
	Light dirLights[DirLightNum];

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���s�����A�_�����̔z����܂Ƃ߂đ���)
	struct ConstBufferData
	{
		// �����̐F
		XMFLOAT3 ambientColor;
		float pad1;

		// ���s�����̔z��
		Light::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
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

	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3& pos);
	void SetPointLightColor(int index, const XMFLOAT3& color);
	void SetPointLightAtten(int index, const XMFLOAT3& atten);
};

