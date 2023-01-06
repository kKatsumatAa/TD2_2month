#pragma once
#include "DirectX.h"


class TextureManager
{
private:
	static int count;



	TextureManager();

	~TextureManager();


public:
	static UINT64 whiteTexHandle;


	//���\�[�X�ݒ�
	static D3D12_RESOURCE_DESC resDesc;
	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	static ComPtr < ID3D12DescriptorHeap> srvHeap;
	static D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	static const int srvCount = 512;
	static ComPtr<ID3D12Resource> texBuff[srvCount];

	//SRV�̍ő��
	static const size_t kMaxSRVCount;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	static D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc;

	//�f�X�N���v�^�����W�̐ݒ�
	static D3D12_DESCRIPTOR_RANGE descriptorRange;

public:
	//�R�s�[�R���X�g���N�^�𖳌�
	TextureManager(const TextureManager& obj) = delete;
	//������Z�q��
	TextureManager& operator=(const TextureManager& obj) = delete;

	static TextureManager& GetInstance();

	//�f�X�N���v�^�q�[�v������
	static void InitializeDescriptorHeap();
	static void LoadGraph(const wchar_t* name, UINT64& textureHandle);
};

