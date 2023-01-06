#include "Material.h"
#include <DirectXTex.h>
#include <cassert>
#include "TextureManager.h"

using namespace DirectX;
using namespace std;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Material::device = nullptr;

void Material::StaticInitialize(ID3D12Device* device)
{
	// �ď������`�F�b�N
	assert(!Material::device);

	Material::device = device;
}

Material* Material::Create()
{
	Material* instance = new Material;

	instance->Initialize();

	return instance;
}

void Material::Initialize()
{
	// �萔�o�b�t�@�̐���
	CreateConstantBuffer();
}

void Material::CreateConstantBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ResourceProperties(cbResourceDesc,
		((UINT)sizeof(Material::ConstBufferDataMaterial2) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/);
	//�萔�o�b�t�@�̐���
	BuffProperties(cbHeapProp, cbResourceDesc, &constBuff);
}

void Material::LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle)
{
	// �e�N�X�`���Ȃ�
	if (textureFilename.size() == 0) {
		textureFilename = "image/white.png";
	}

	cpuDescHandleSRV = cpuHandle;
	gpuDescHandleSRV = gpuHandle;

	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �t�@�C���p�X������
	string filepath = directoryPath + textureFilename;

	const char* name = filepath.c_str();
	wchar_t wchar[128];
	size_t size = _countof(wchar);
	mbstowcs_s(&size, wchar, name, size);
	TextureManager::LoadGraph(wchar, textureHandle);
}

void Material::Update()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataMaterial2* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}

const CD3DX12_GPU_DESCRIPTOR_HANDLE& Material::GetGpuHandle()
{
	gpuDescHandleSRV.ptr = textureHandle;
	return gpuDescHandleSRV;
}
