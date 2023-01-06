#pragma once
#include"TextureManager.h"

class ConstBuffTransform
{
private:
	//�萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world;    //���[���h�s��
		XMFLOAT3 cameraPos;//�J�������W�i���[���h���W�j
	};

public:
	ComPtr < ID3D12Resource> constBuffTransform = nullptr;//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ConstBufferDataTransform* constMapTransform = nullptr;//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^

	ConstBuffTransform(/*D3D12_RESOURCE_DESC& resDesc,*/ /*Directx& directx*/);
	void Initialize(Directx& directx);
};