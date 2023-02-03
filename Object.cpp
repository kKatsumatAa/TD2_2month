#include "Object.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//�}�`�̃N���X
Primitive primitive;


//�e�N�X�`��

// ���_���C�A�E�g
D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
{//xyz���W
 "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
 D3D12_APPEND_ALIGNED_ELEMENT,
 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
}, // (1�s�ŏ������ق������₷��)

	{//�@���x�N�g��
 "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
 D3D12_APPEND_ALIGNED_ELEMENT,
 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
}, // (1�s�ŏ������ق������₷��)

{//uv���W
 "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
 D3D12_APPEND_ALIGNED_ELEMENT,
 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
} // (1�s�ŏ������ق������₷��)
};

//sprite
D3D12_INPUT_ELEMENT_DESC inputLayoutSprite[2] = {
	{//xyz���W
	 "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1�s�ŏ������ق������₷��)

	{//uv���W
	 "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	} // (1�s�ŏ������ق������₷��)
};
PipeLineSet pipelineSet;
//al4_02_02
PipeLineSet pipelineSetM;

//���[�g�p�����[�^�̐ݒ�
D3D12_ROOT_PARAMETER rootParams[6] = {};

// �p�C�v�����X�e�[�g�̐���
ComPtr < ID3D12PipelineState> pipelineState[3] = { nullptr };
// ���[�g�V�O�l�`��
ComPtr<ID3D12RootSignature> rootSignature;
// �O���t�B�b�N�X�p�C�v���C���ݒ�
D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

//static
LightManager* Object::lightManager = nullptr;

PostPera postPera;

ComPtr <ID3D12Resource> Object::effectFlagsBuff = nullptr;
EffectConstBuffer* Object::mapEffectFlagsBuff = nullptr;
EffectConstBuffer Object::effectFlags;


struct weightMap
{
	XMFLOAT4 mappedWeight[2];
};

void DrawInitialize()
{
	//�e�N�X�`���p�̃f�X�N���v�^�q�[�v������
	TextureManager::GetInstance().InitializeDescriptorHeap();

	//�}�`�N���X��
	primitive.Initialize();

	//���[�g�p�����[�^�̐ݒ�
	//�萔�o�b�t�@0�ԁi�F�j
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�(b0)
	rootParams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0�ԁi�e�N�X�`���j
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//�f�X�N���v�^
	rootParams[1].DescriptorTable.pDescriptorRanges = &TextureManager::GetInstance().descriptorRange;//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//�V��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1��(�s��)
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�(b1)
	rootParams[2].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@2�ԁi�}�e���A���j
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[3].Descriptor.ShaderRegister = 2;//�萔�o�b�t�@�ԍ�(b2)
	rootParams[3].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@3��(���C�g�p)
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[4].Descriptor.ShaderRegister = 3;//�萔�o�b�t�@�ԍ�(b3)
	rootParams[4].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@4��(��ʌ��ʃt���O)
	rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[5].Descriptor.ShaderRegister = 4;//�萔�o�b�t�@�ԍ�(b4)
	rootParams[5].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

	// �p�C�v�����X�e�[�g�̐���
	PipeLineState(D3D12_FILL_MODE_SOLID, pipelineState->GetAddressOf(), rootSignature.GetAddressOf(), vsBlob, psBlob);

	PipeLineState(D3D12_FILL_MODE_WIREFRAME, (pipelineState + 1)->GetAddressOf(), rootSignature.GetAddressOf(), vsBlob, psBlob);

	//line
	PipeLineState(D3D12_FILL_MODE_WIREFRAME, (pipelineState + 2)->GetAddressOf(), rootSignature.GetAddressOf(), vsBlob, psBlob, LINE);

	//sprite�p
	PipeLineState(D3D12_FILL_MODE_SOLID, pipelineSet.pipelineState.GetAddressOf(),
		pipelineSet.rootSignature.GetAddressOf(), pipelineSet.vsBlob,
		pipelineSet.psBlob, SPRITE);

	//model�p
	PipeLineState(D3D12_FILL_MODE_SOLID, pipelineSetM.pipelineState.GetAddressOf(),
		pipelineSetM.rootSignature.GetAddressOf(), pipelineSetM.vsBlob,
		pipelineSetM.psBlob, MODEL);

	postPera.Initialize();

	//��ʌ��ʗp
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
		//���\�[�X�ݒ�
		ResourceProperties(cbResourceDesc,
			((UINT)sizeof(EffectConstBuffer) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/);
		//�萔�o�b�t�@�̐���
		BuffProperties(cbHeapProp, cbResourceDesc, &Object::effectFlagsBuff);
		//�萔�o�b�t�@�̃}�b�s���O
		Directx::GetInstance().result = Object::effectFlagsBuff->Map(0, nullptr, (void**)&Object::mapEffectFlagsBuff);//�}�b�s���O
		assert(SUCCEEDED(Directx::GetInstance().result));
	}
}

//----------------------------------------------------------------


//-------------------------------------------------------------------

Object::Object()
{
	//�s��
	cbt.Initialize(Directx::GetInstance());

	//03_02
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ResourceProperties(cbResourceDesc,
		((UINT)sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/);
	//�萔�o�b�t�@�̐���
	BuffProperties(cbHeapProp, cbResourceDesc, &constBuffMaterial);
	//�萔�o�b�t�@�̃}�b�s���O
	Directx::GetInstance().result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(Directx::GetInstance().result));
}

void Object::Update(const int& indexNum, const int& pipelineNum, const UINT64 textureHandle, const ConstBuffTransform& constBuffTransform,
	Model* model, const bool& primitiveMode)
{
	worldMat->SetWorld();

	//�ϊ��s���GPU�ɑ��M
	if (worldMat->parent != nullptr && indexNum != SPRITE)//�e������ꍇ
	{
		worldMat->matWorld *= worldMat->parent->matWorld;
		XMMATRIX matW;
		matW = { (float)worldMat->matWorld.m[0][0],(float)worldMat->matWorld.m[0][1],(float)worldMat->matWorld.m[0][2],(float)worldMat->matWorld.m[0][3],
				 (float)worldMat->matWorld.m[1][0],(float)worldMat->matWorld.m[1][1],(float)worldMat->matWorld.m[1][2],(float)worldMat->matWorld.m[1][3],
				 (float)worldMat->matWorld.m[2][0],(float)worldMat->matWorld.m[2][1],(float)worldMat->matWorld.m[2][2],(float)worldMat->matWorld.m[2][3],
				 (float)worldMat->matWorld.m[3][0],(float)worldMat->matWorld.m[3][1],(float)worldMat->matWorld.m[3][2],(float)worldMat->matWorld.m[3][3] };

		cbt.constMapTransform->world = matW;
		cbt.constMapTransform->viewproj = view->matView * projection->matProjection;
		XMFLOAT3 cPos = { view->eye.x,view->eye.y,view->eye.z };
		cbt.constMapTransform->cameraPos = cPos;

	}
	else if (indexNum != SPRITE)//�e�����Ȃ��ꍇ
	{
		XMMATRIX matW;
		matW = { (float)worldMat->matWorld.m[0][0],(float)worldMat->matWorld.m[0][1],(float)worldMat->matWorld.m[0][2],(float)worldMat->matWorld.m[0][3],
				 (float)worldMat->matWorld.m[1][0],(float)worldMat->matWorld.m[1][1],(float)worldMat->matWorld.m[1][2],(float)worldMat->matWorld.m[1][3],
				 (float)worldMat->matWorld.m[2][0],(float)worldMat->matWorld.m[2][1],(float)worldMat->matWorld.m[2][2],(float)worldMat->matWorld.m[2][3],
				 (float)worldMat->matWorld.m[3][0],(float)worldMat->matWorld.m[3][1],(float)worldMat->matWorld.m[3][2],(float)worldMat->matWorld.m[3][3] };

		cbt.constMapTransform->world = matW;
		cbt.constMapTransform->viewproj = view->matView * projection->matProjection;
		XMFLOAT3 cPos = { view->eye.x,view->eye.y,view->eye.z };
		cbt.constMapTransform->cameraPos = cPos;
	}

	//��ʌ��ʗp
	{
		mapEffectFlagsBuff->isEmboss = this->effectFlags.isEmboss;
		mapEffectFlagsBuff->isFog = this->effectFlags.isFog;
		mapEffectFlagsBuff->isGaussian = this->effectFlags.isGaussian;
		mapEffectFlagsBuff->isGaussian2 = this->effectFlags.isGaussian2;
		mapEffectFlagsBuff->isGradation = this->effectFlags.isGradation;
		mapEffectFlagsBuff->isOutLine = this->effectFlags.isOutLine;
		mapEffectFlagsBuff->isSharpness = this->effectFlags.isSharpness;
		mapEffectFlagsBuff->isVignette = this->effectFlags.isVignette;
		mapEffectFlagsBuff->isBarrelCurve = this->effectFlags.isBarrelCurve;
		mapEffectFlagsBuff->isScanningLine = this->effectFlags.isScanningLine;
	}

	//�e�N�X�`����ݒ肵�Ă��Ȃ�������
	UINT64 textureHandle_;

	if (textureHandle == NULL)
	{
		textureHandle_ = TextureManager::GetInstance().whiteTexHandle;
	}
	else
	{
		textureHandle_ = textureHandle;
	}

	if (indexNum == TRIANGLE)
	{
		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		Vertex* vertMap = nullptr;
		Directx::GetInstance().result = primitive.vertBuffTriangle->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(Directx::GetInstance().result));
		// �S���_�ɑ΂���
		for (int i = 0; i < _countof(primitive.verticesTriangle); i++) {

			vertMap[i] = primitive.verticesTriangle[i]; // ���W���R�s�[

		}
		// �q���������
		primitive.vertBuffTriangle->Unmap(0, nullptr);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineState[pipelineNum].Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p���X�g

		Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &primitive.vbTriangleView);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		Directx::GetInstance().GetCommandList()->IASetIndexBuffer(&primitive.ibViewTriangle);

		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->DrawIndexedInstanced(_countof(indicesTriangle), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��

	}
	else if (indexNum == BOX)
	{
		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		Vertex* vertMap = nullptr;
		Directx::GetInstance().result = primitive.vertBuffBox->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(Directx::GetInstance().result));
		// �S���_�ɑ΂���
		for (int i = 0; i < _countof(primitive.verticesBox); i++) {

			vertMap[i] = primitive.verticesBox[i]; // ���W���R�s�[

		}
		// �q���������
		primitive.vertBuffBox->Unmap(0, nullptr);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineState[pipelineNum].Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p���X�g

		Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &primitive.vbBoxView);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		Directx::GetInstance().GetCommandList()->IASetIndexBuffer(&primitive.ibViewBox);

		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->DrawIndexedInstanced(_countof(indicesBox), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}
	else if (indexNum == CUBE)
	{
		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		Vertex* vertMap = nullptr;
		Directx::GetInstance().result = primitive.vertBuffCube->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(Directx::GetInstance().result));
		// �S���_�ɑ΂���
		for (int i = 0; i < _countof(primitive.verticesCube); i++) {

			vertMap[i] = primitive.verticesCube[i]; // ���W���R�s�[

		}
		// �q���������
		primitive.vertBuffCube->Unmap(0, nullptr);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineState[pipelineNum].Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p���X�g

		Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &primitive.vbCubeView);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		Directx::GetInstance().GetCommandList()->IASetIndexBuffer(&primitive.ibViewCube);

		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->DrawIndexedInstanced(_countof(indicesCube), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}
	else if (indexNum == LINE)
	{
		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		Vertex* vertMap = nullptr;
		Directx::GetInstance().result = primitive.vertBuffLine->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(Directx::GetInstance().result));
		// �S���_�ɑ΂���
		for (int i = 0; i < _countof(primitive.verticesLine); i++) {

			vertMap[i] = primitive.verticesLine[i]; // ���W���R�s�[

		}
		// �q���������
		primitive.vertBuffLine->Unmap(0, nullptr);

		//�p�C�v���C��
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineState[2].Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST); // ��

		Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &primitive.vbLineView);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		Directx::GetInstance().GetCommandList()->IASetIndexBuffer(&primitive.ibViewLine);

		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->DrawIndexedInstanced(_countof(indicesLine), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}
	else if (indexNum == CIRCLE)
	{
		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		Vertex* vertMap = nullptr;
		Directx::GetInstance().result = primitive.vertBuffCircle->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(Directx::GetInstance().result));
		// �S���_�ɑ΂���
		for (int i = 0; i < _countof(primitive.verticesCircle); i++) {

			vertMap[i] = primitive.verticesCircle[i]; // ���W���R�s�[

		}
		// �q���������
		primitive.vertBuffCircle->Unmap(0, nullptr);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineState[pipelineNum].Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p���X�g

		Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &primitive.vbCircleView);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		Directx::GetInstance().GetCommandList()->IASetIndexBuffer(&primitive.ibViewCircle);

		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->DrawIndexedInstanced(_countof(indicesCircle), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}
	else if (indexNum == SPHERE)
	{

		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		Vertex* vertMap = nullptr;
		Directx::GetInstance().result = primitive.vertBuffSphere->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(Directx::GetInstance().result));
		// �S���_�ɑ΂���
		for (int i = 0; i < _countof(primitive.verticesSphere); i++) {

			vertMap[i] = primitive.verticesSphere[i]; // ���W���R�s�[

		}
		// �q���������
		primitive.vertBuffSphere->Unmap(0, nullptr);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineState[pipelineNum].Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�X�g���b�v
		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &primitive.vbViewSphere);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		Directx::GetInstance().GetCommandList()->IASetIndexBuffer(&primitive.ibViewSphere);

		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->DrawIndexedInstanced(_countof(primitive.indicesSphere), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}
	else if (indexNum == SPRITE)
	{
		SpriteCommonBeginDraw(&pipelineSet);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());


		//04_02
		{
			//SRV�q�[�v�̐ݒ�R�}���h
			Directx::GetInstance().GetCommandList()->SetDescriptorHeaps(1, TextureManager::GetInstance().srvHeap.GetAddressOf());
			//SRV�q�[�v�̐擪�n���h�����擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
			srvGpuHandle.ptr = textureHandle_;
			//(�C���X�^���X�œǂݍ��񂾃e�N�X�`���p��SRV���w��)
			Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		sprite->SpriteDraw();
	}
	else if (indexNum == MODEL)
	{
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetPipelineState(pipelineSetM.pipelineState.Get());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(pipelineSetM.rootSignature.Get());

		Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		lightManager->Draw(4);

		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform.constBuffTransform->GetGPUVirtualAddress());

		Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(5, effectFlagsBuff->GetGPUVirtualAddress());

		//���f���p�`��
		model->Draw();
	}
}

void Object::DrawPera()
{
	//�|�X�g�G�t�F�N�g�p
	postPera.Draw(effectFlags);
}

void Object::DrawTriangle(/*XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3,*/
	WorldMat* world, ViewMat* view, ProjectionMat* projection, XMFLOAT4 color, const UINT64 textureHandle, const int& pipelineNum)
{
	//constBuffTransfer({ 0.01f,0.05f,0.1f,0 });
	this->worldMat = world;
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(TRIANGLE, pipelineNum, textureHandle, cbt);
}

void Object::DrawBox(WorldMat* world, ViewMat* view, ProjectionMat* projection,/*XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, */
	XMFLOAT4 color, const UINT64 textureHandle, const int& pipelineNum)
{
	//constBuffTransfer({ 0.01f,0.05f,0.1f,0 });
	this->worldMat = world;
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(BOX, pipelineNum, textureHandle, cbt);
}

void Object::DrawBoxSprite(const Vec3& pos, const float& scale,
	XMFLOAT4 color, const UINT64 textureHandle, const Vec2& ancorUV, const bool& isReverseX, const bool& isReverseY,
	float rotation, const int& pipelineNum)
{
	if (sprite == nullptr)
	{
		sprite = new Sprite();
		//�X�v���C�g�N���X�̏�����
		sprite->Initialize();
	}
	sprite->Update(pos, scale, color, textureHandle, ancorUV, isReverseX, isReverseY, rotation, &cbt, constMapMaterial);

	Update(SPRITE, pipelineNum, textureHandle, cbt);
}




void Object::DrawClippingBoxSprite(const Vec3& leftTop, const float& scale, const XMFLOAT2& UVleftTop, const XMFLOAT2& UVlength,
	XMFLOAT4 color, const UINT64 textureHandle, bool isPosLeftTop, const bool& isReverseX, const bool& isReverseY,
	float rotation, const int& pipelineNum)
{
	if (sprite == nullptr)
	{
		sprite = new Sprite();
		//�X�v���C�g�N���X�̏�����
		sprite->Initialize();
	}
	sprite->UpdateClipping(leftTop, scale, UVleftTop, UVlength, color, textureHandle,
		isPosLeftTop, isReverseX, isReverseY, rotation, &cbt, constMapMaterial);

	Update(SPRITE, pipelineNum, textureHandle, cbt);
}

void Object::DrawCube3D(WorldMat* world, ViewMat* view, ProjectionMat* projection, XMFLOAT4 color, const UINT64 textureHandle, const int& pipelineNum)
{
	this->worldMat = world;
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(CUBE, pipelineNum, textureHandle, cbt);
}

void Object::DrawLine(/*const Vec3& pos1, const Vec3& pos2,*/ WorldMat* world, ViewMat* view, ProjectionMat* projection, const XMFLOAT4& color,
	const UINT64 textureHandle, const int& pipelineNum)
{
	this->worldMat = world;
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(LINE, pipelineNum, textureHandle, cbt, nullptr, false);
}

void Object::DrawCircle(float radius, WorldMat* world, ViewMat* view, ProjectionMat* projection,
	XMFLOAT4 color, const UINT64 textureHandle, const int& pipelineNum)
{
	this->worldMat = world;
	this->worldMat->scale = { radius,radius,radius };
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(CIRCLE, pipelineNum, textureHandle, cbt);
}

void Object::DrawSphere(WorldMat* world, ViewMat* view, ProjectionMat* projection,
	XMFLOAT4 color, const UINT64 textureHandle, const int& pipelineNum)
{
	this->worldMat = world;
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(SPHERE, pipelineNum, textureHandle, cbt);
}

void Object::DrawModel(WorldMat* world, ViewMat* view, ProjectionMat* projection,
	Model* model, XMFLOAT4 color, const int& pipelineNum)
{
	this->worldMat = world;
	this->view = view;
	this->projection = projection;

	constMapMaterial->color = color;

	Update(MODEL, pipelineNum, NULL, cbt, model);
}

void PipeLineState(const D3D12_FILL_MODE& fillMode, ID3D12PipelineState** pipelineState, ID3D12RootSignature** rootSig,
	ID3DBlob* vsBlob, ID3DBlob* psBlob, const int& indexNum)
{
	if (indexNum == SPRITE)
	{
		// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		Directx::GetInstance().result = D3DCompileFromFile(
			L"Resources/shaders/SpriteVS.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&vsBlob, &errorBlob);

		// �G���[�Ȃ�
		Error(FAILED(Directx::GetInstance().result));

		// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		Directx::GetInstance().result = D3DCompileFromFile(
			L"Resources/shaders/SpritePS.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&psBlob, &errorBlob);

		// �G���[�Ȃ�
		Error(FAILED(Directx::GetInstance().result));
	}
	else if (indexNum == MODEL)
	{
		// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		Directx::GetInstance().result = D3DCompileFromFile(
			L"Resources/shaders/OBJVertexShader.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&vsBlob, &errorBlob);

		// �G���[�Ȃ�
		Error(FAILED(Directx::GetInstance().result));

		// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		Directx::GetInstance().result = D3DCompileFromFile(
			L"Resources/shaders/OBJPixelShader.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&psBlob, &errorBlob);

		// �G���[�Ȃ�
		Error(FAILED(Directx::GetInstance().result));
	}
	else
	{
		// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		Directx::GetInstance().result = D3DCompileFromFile(
			L"Resources/shaders/BasicVS.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&vsBlob, &errorBlob);

		// �G���[�Ȃ�
		Error(FAILED(Directx::GetInstance().result));

		// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		Directx::GetInstance().result = D3DCompileFromFile(
			L"Resources/shaders/BasicPS.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&psBlob, &errorBlob);

		// �G���[�Ȃ�
		Error(FAILED(Directx::GetInstance().result));
	}

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	if (indexNum == SPRITE)
	{
		pipelineDesc.RasterizerState = D3D12_RASTERIZER_DESC();
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
	else
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʃJ�����O

	pipelineDesc.RasterizerState.FillMode = fillMode; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	Blend(D3D12_BLEND_OP_ADD, false, true);

	// ���_���C�A�E�g�̐ݒ�
	if (indexNum == SPRITE)
	{
		pipelineDesc.InputLayout.pInputElementDescs = inputLayoutSprite;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayoutSprite);
	}
	else
	{
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	}

	// �}�`�̌`��ݒ�
	if (indexNum == LINE)
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	else
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//04_02
	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  //���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  //�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  //���s���J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;    //�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                  //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                               //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Directx::GetInstance().result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(Directx::GetInstance().result));
	Directx::GetInstance().result = Directx::GetInstance().GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSig));
	assert(SUCCEEDED(Directx::GetInstance().result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = *rootSig;

	//06_01
	//�f�v�X�X�e���V���X�e�[�g
	pipelineDesc.DepthStencilState = D3D12_DEPTH_STENCIL_DESC();
	pipelineDesc.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	if (indexNum == SPRITE)
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��������΍��i
	else
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	Directx::GetInstance().result = Directx::GetInstance().GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState));
	//assert(SUCCEEDED(Directx::GetInstance().result));
}

void Blend(const D3D12_BLEND_OP& blendMode, const bool& Inversion, const bool& Translucent)
{
	//���ʐݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��
	blendDesc.BlendEnable = true;//�u�����h��L��
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��
	//��������
	blendDesc.BlendOp = blendMode;
	if (Inversion)//���]
	{
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blendDesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�
	}
	else if (Translucent)//������
	{
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//1.0f-�f�X�g�J���[�̒l
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}
	else
	{
		blendDesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
		blendDesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��
	}
}

void Object::constBuffTransfer(const XMFLOAT4& plusRGBA)
{
	if (constMapMaterial->color.x <= 1.0f && constMapMaterial->color.x >= 0.0f)
		constMapMaterial->color.x += plusRGBA.x;
	if (constMapMaterial->color.y <= 1.0f && constMapMaterial->color.y >= 0.0f)
		constMapMaterial->color.y += plusRGBA.y;
	if (constMapMaterial->color.z <= 1.0f && constMapMaterial->color.z >= 0.0f)
		constMapMaterial->color.z += plusRGBA.z;
	if (constMapMaterial->color.w <= 1.0f && constMapMaterial->color.w >= 0.0f)
		constMapMaterial->color.w += plusRGBA.w;
}



void SetNormDigitalMat(XMMATRIX& mat)
{
	mat.r[0].m128_f32[0] = 2.0f / WindowsApp::GetInstance().window_width;
	mat.r[1].m128_f32[1] = -2.0f / WindowsApp::GetInstance().window_height;
	mat.r[3].m128_f32[0] = -1.0f;
	mat.r[3].m128_f32[1] = 1.0f;
}

void Error(const bool& filed)
{
	if (filed)
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}
