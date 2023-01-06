#include "ConstBuffTransform.h"

ConstBuffTransform::ConstBuffTransform(/*D3D12_RESOURCE_DESC& resDesc,*//* Directx& directx*/)
{

}

void ConstBuffTransform::Initialize(Directx& directx)
{
	//05_02
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};

		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/;						//���_�f�[�^�S�̂̃T�C�Y
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		directx.result = directx.GetDevice()->CreateCommittedResource(
			&cbHeapProp,//�q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(directx.result));

	}
	//�萔�o�b�t�@�̃}�b�s���O
	directx.result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//�}�b�s���O
	assert(SUCCEEDED(directx.result));
}
