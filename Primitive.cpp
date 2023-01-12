#include "Primitive.h"
#include "Util.h"



UINT sizeVB;

void Primitive::Initialize()
{
	InitializeTriangle();
	InitializeBox();
	InitializeCircle();
	InitializeLine();
	InitializeCube();
	InitializeSphere();
}

void Primitive::InitializeTriangle()
{
	verticesTriangle[0] = { {-1.0,-1.0,0},{verticesTriangle[0].normal},{0.0f,1.0f} };//����
	verticesTriangle[1] = { {0,1.0,0},{verticesTriangle[1].normal},{0.5f,0.0f} };//��
	verticesTriangle[2] = { {1.0,-1.0,0},{verticesTriangle[2].normal},{1.0f,1.0f} };//�E��


	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(verticesTriangle[0]) * _countof(verticesTriangle));

	//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p

	ResourceProperties(TextureManager::GetInstance().resDesc, sizeVB);

	//���_�o�b�t�@�̐���
	BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &vertBuffTriangle);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbTriangleView.BufferLocation = vertBuffTriangle->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbTriangleView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbTriangleView.StrideInBytes = sizeof(verticesTriangle[0]);

	{
		//03_04
		//�C���f�b�N�X�f�[�^
		//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicesTriangle));

		//���\�[�X�ݒ�
		ResourceProperties(TextureManager::GetInstance().resDesc, sizeIB);
		//�C���f�b�N�X�o�b�t�@�̍쐬
		ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &indexBuff);
		//�C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		Directx::GetInstance().result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indicesTriangle); i++)
		{
			indexMap[i] = indicesTriangle[i];//�C���f�b�N�X���R�s�[

		}
		//�}�b�s���O������
		indexBuff->Unmap(0, nullptr);

		//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
		ibViewTriangle.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibViewTriangle.Format = DXGI_FORMAT_R16_UINT;
		ibViewTriangle.SizeInBytes = sizeIB;
	}

	//06_03
	{
		for (int i = 0; i < _countof(indicesTriangle) / 3; i++)
		{//�O�p�`����ƂɌv�Z
			//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
			unsigned short index0 = indicesTriangle[i * 3 + 0];
			unsigned short index1 = indicesTriangle[i * 3 + 1];
			unsigned short index2 = indicesTriangle[i * 3 + 2];
			//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
			XMVECTOR p0 = XMLoadFloat3(&verticesTriangle[index0].pos);
			XMVECTOR p1 = XMLoadFloat3(&verticesTriangle[index1].pos);
			XMVECTOR p2 = XMLoadFloat3(&verticesTriangle[index2].pos);
			//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z
			XMVECTOR v1 = XMVectorSubtract(p1, p0);
			XMVECTOR v2 = XMVectorSubtract(p2, p0);
			//�O�ρi�����ȃx�N�g���j
			XMVECTOR normal = XMVector3Cross(XMVector3Normalize(v1), XMVector3Normalize(v2));
			//���߂��@���𒸓_�f�[�^�ɑ��
			XMStoreFloat3(&verticesTriangle[index0].normal, normal);
			XMStoreFloat3(&verticesTriangle[index1].normal, normal);
			XMStoreFloat3(&verticesTriangle[index2].normal, normal);
		}
	}
}

void Primitive::InitializeBox()
{
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(verticesBox[0]) * _countof(verticesBox));

	//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p

	ResourceProperties(TextureManager::GetInstance().resDesc, sizeVB);

	//���_�o�b�t�@�̐���
	BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &vertBuffBox);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbBoxView.BufferLocation = vertBuffBox->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbBoxView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbBoxView.StrideInBytes = sizeof(verticesBox[0]);

	{
		//03_04
		//�C���f�b�N�X�f�[�^
		//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicesBox));

		//���\�[�X�ݒ�
		ResourceProperties(TextureManager::GetInstance().resDesc, sizeIB);
		//�C���f�b�N�X�o�b�t�@�̍쐬
		ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &indexBuff);
		//�C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		Directx::GetInstance().result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indicesBox); i++)
		{
			indexMap[i] = indicesBox[i];//�C���f�b�N�X���R�s�[

		}
		//�}�b�s���O������
		indexBuff->Unmap(0, nullptr);

		//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
		ibViewBox.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibViewBox.Format = DXGI_FORMAT_R16_UINT;
		ibViewBox.SizeInBytes = sizeIB;
	}

	//06_03
	{
		for (int i = 0; i < _countof(indicesBox) / 3; i++)
		{//�O�p�`����ƂɌv�Z
			//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
			unsigned short index0 = indicesBox[i * 3 + 0];
			unsigned short index1 = indicesBox[i * 3 + 1];
			unsigned short index2 = indicesBox[i * 3 + 2];
			//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
			XMVECTOR p0 = XMLoadFloat3(&verticesBox[index0].pos);
			XMVECTOR p1 = XMLoadFloat3(&verticesBox[index1].pos);
			XMVECTOR p2 = XMLoadFloat3(&verticesBox[index2].pos);
			//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z
			XMVECTOR v1 = XMVectorSubtract(p1, p0);
			XMVECTOR v2 = XMVectorSubtract(p2, p0);
			//�O�ρi�����ȃx�N�g���j
			XMVECTOR normal = XMVector3Cross(XMVector3Normalize(v1), XMVector3Normalize(v2));
			//���߂��@���𒸓_�f�[�^�ɑ��
			XMStoreFloat3(&verticesBox[index0].normal, normal);
			XMStoreFloat3(&verticesBox[index1].normal, normal);
			XMStoreFloat3(&verticesBox[index2].normal, normal);
		}
	}
}

void Primitive::InitializeCircle()
{
	verticesCircle[0].pos = { 0.0f,0.0f,0.0f };

	static float count = _countof(verticesCircle) - 2;//���S�_�Ə����̓_�̓J�E���g���Ȃ�

	for (int i = 1; i < _countof(verticesCircle); i++)
	{
		verticesCircle[i].pos = { 1.0f * cosf(AngletoRadi(360 / count) * (i - 1)),1.0f * sinf(AngletoRadi(360 / count) * (i - 1)),0 };
	}

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(verticesCircle[0]) * _countof(verticesCircle));

	//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p

	ResourceProperties(TextureManager::GetInstance().resDesc, sizeVB);

	//���_�o�b�t�@�̐���
	BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &vertBuffCircle);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbCircleView.BufferLocation = vertBuffCircle->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbCircleView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbCircleView.StrideInBytes = sizeof(verticesCircle[0]);

	{
		//03_04
		//�C���f�b�N�X�f�[�^
		//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicesCircle));

		//���\�[�X�ݒ�
		ResourceProperties(TextureManager::GetInstance().resDesc, sizeIB);
		//�C���f�b�N�X�o�b�t�@�̍쐬
		ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &indexBuff);
		//�C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		Directx::GetInstance().result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indicesCircle); i++)
		{
			indexMap[i] = indicesCircle[i];//�C���f�b�N�X���R�s�[

		}
		//�}�b�s���O������
		indexBuff->Unmap(0, nullptr);

		//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
		ibViewCircle.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibViewCircle.Format = DXGI_FORMAT_R16_UINT;
		ibViewCircle.SizeInBytes = sizeIB;
	}

	//06_03
	{
		for (int i = 0; i < _countof(indicesCircle) / 3; i++)
		{//�O�p�`����ƂɌv�Z
			//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
			unsigned short index0 = indicesCircle[i * 3 + 0];
			unsigned short index1 = indicesCircle[i * 3 + 1];
			unsigned short index2 = indicesCircle[i * 3 + 2];
			//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
			XMVECTOR p0 = XMLoadFloat3(&verticesCircle[index0].pos);
			XMVECTOR p1 = XMLoadFloat3(&verticesCircle[index1].pos);
			XMVECTOR p2 = XMLoadFloat3(&verticesCircle[index2].pos);
			//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z
			XMVECTOR v1 = XMVectorSubtract(p1, p0);
			XMVECTOR v2 = XMVectorSubtract(p2, p0);
			//�O�ρi�����ȃx�N�g���j
			XMVECTOR normal = XMVector3Cross(XMVector3Normalize(v1), XMVector3Normalize(v2));
			//���߂��@���𒸓_�f�[�^�ɑ��
			XMStoreFloat3(&verticesCircle[index0].normal, normal);
			XMStoreFloat3(&verticesCircle[index1].normal, normal);
			XMStoreFloat3(&verticesCircle[index2].normal, normal);
		}
	}
}

void Primitive::InitializeCube()
{
	{
		//��O
		verticesCube[0] = { {-1.0f,-1.0f,-1.0f},{verticesCube[0].normal},{0.0f,1.0f} };//����
		verticesCube[1] = { {-1.0f,1.0f, -1.0f},{verticesCube[1].normal},{0.0f,0.0f} };//����
		verticesCube[2] = { {1.0f,-1.0f, -1.0f},{verticesCube[2].normal},{1.0f,1.0f} };//�E��
		verticesCube[3] = { {1.0f,1.0f,  -1.0f},{verticesCube[3].normal},{1.0f,0.0f} };//�E��

		verticesCube[4] = { {-1.0f,-1.0f,1.0f},{verticesCube[4].normal},{0.0f,1.0f} };//����
		verticesCube[5] = { {-1.0f,1.0f, 1.0f},{verticesCube[5].normal},{0.0f,0.0f} };//����
		verticesCube[6] = { {1.0f,-1.0f, 1.0f},{verticesCube[6].normal},{1.0f,1.0f} };//�E��
		verticesCube[7] = { {1.0f,1.0f,  1.0f},{verticesCube[7].normal},{1.0f,0.0f} };//�E��
			//��
		verticesCube[8] = { {1.0f,1.0f,-1.0f},{verticesCube[8].normal},{0.0f,1.0f} };//����
		verticesCube[9] = { {1.0f,1.0f, 1.0f},{verticesCube[9].normal},{0.0f,0.0f} };//����
		verticesCube[10] = { {-1.0f,1.0f, -1.0f},{verticesCube[10].normal},{1.0f,1.0f} };//�E��
		verticesCube[11] = { {-1.0f,1.0f, 1.0f},{verticesCube[11].normal},{1.0f,0.0f} };//�E��

		verticesCube[12] = { {1.0f,-1.0f,-1.0f},{verticesCube[12].normal},{0.0f,1.0f} };//����
		verticesCube[13] = { {1.0f,-1.0f, 1.0f},{verticesCube[13].normal},{0.0f,0.0f} };//����
		verticesCube[14] = { {-1.0f,-1.0f, -1.0f},{verticesCube[14].normal},{1.0f,1.0f} };//�E��
		verticesCube[15] = { {-1.0f,-1.0f, 1.0f},{verticesCube[15].normal},{1.0f,0.0f} };//�E��

		verticesCube[16] = { {-1.0f,-1.0f,-1.0f},{verticesCube[16].normal},{0.0f,1.0f} };//����
		verticesCube[17] = { {-1.0f,-1.0f, 1.0f},{verticesCube[17].normal},{0.0f,0.0f} };//����
		verticesCube[18] = { {-1.0f,1.0f, -1.0f},{verticesCube[18].normal},{1.0f,1.0f} };//�E��
		verticesCube[19] = { {-1.0f,1.0f,  1.0f},{verticesCube[19].normal},{1.0f,0.0f} };//�E��

		verticesCube[20] = { {1.0f,-1.0f,-1.0f},{verticesCube[20].normal},{0.0f,1.0f} };//����
		verticesCube[21] = { {1.0f,-1.0f, 1.0f},{verticesCube[21].normal},{0.0f,0.0f} };//����
		verticesCube[22] = { {1.0f,1.0f, -1.0f},{verticesCube[22].normal},{1.0f,1.0f} };//�E��
		verticesCube[23] = { {1.0f,1.0f,  1.0f},{verticesCube[23].normal},{1.0f,0.0f} };//�E��;//����
	}

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(verticesCube[0]) * _countof(verticesCube));

	//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p

	ResourceProperties(TextureManager::GetInstance().resDesc, sizeVB);

	//���_�o�b�t�@�̐���
	BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &vertBuffCube);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbCubeView.BufferLocation = vertBuffCube->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbCubeView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbCubeView.StrideInBytes = sizeof(verticesCube[0]);

	{
		//03_04
		//�C���f�b�N�X�f�[�^
		//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicesCube));

		//���\�[�X�ݒ�
		ResourceProperties(TextureManager::GetInstance().resDesc, sizeIB);
		//�C���f�b�N�X�o�b�t�@�̍쐬
		ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &indexBuff);
		//�C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		Directx::GetInstance().result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indicesCube); i++)
		{
			indexMap[i] = indicesCube[i];//�C���f�b�N�X���R�s�[

		}
		//�}�b�s���O������
		indexBuff->Unmap(0, nullptr);

		//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
		ibViewCube.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibViewCube.Format = DXGI_FORMAT_R16_UINT;
		ibViewCube.SizeInBytes = sizeIB;
	}


	//06_03
		/*if (indexNum == SPHERE)*/
	{
		for (int i = 0; i < _countof(indicesCube) / 3; i++)
		{//�O�p�`����ƂɌv�Z
			//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
			unsigned short index0 = indicesCube[i * 3 + 0];
			unsigned short index1 = indicesCube[i * 3 + 1];
			unsigned short index2 = indicesCube[i * 3 + 2];
			//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
			XMVECTOR p0 = XMLoadFloat3(&verticesCube[index0].pos);
			XMVECTOR p1 = XMLoadFloat3(&verticesCube[index1].pos);
			XMVECTOR p2 = XMLoadFloat3(&verticesCube[index2].pos);
			//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z
			XMVECTOR v1 = XMVectorSubtract(p1, p0);
			XMVECTOR v2 = XMVectorSubtract(p2, p0);
			//�O�ρi�����ȃx�N�g���j
			XMVECTOR normal = XMVector3Cross(XMVector3Normalize(v1), XMVector3Normalize(v2));
			//���߂��@���𒸓_�f�[�^�ɑ��
			XMStoreFloat3(&verticesCube[index0].normal, normal);
			XMStoreFloat3(&verticesCube[index1].normal, normal);
			XMStoreFloat3(&verticesCube[index2].normal, normal);
		}
	}
}

void Primitive::InitializeLine()
{
	verticesLine[0] = { {0,0,1.0f},{verticesLine[0].normal},{0.0f,1.0f} };//����
	verticesLine[1] = { {0,0,-1.0f},  {verticesLine[1].normal},{0.5f,0.0f} };//��

	{
		//03_04
		//�C���f�b�N�X�f�[�^
		//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicesLine));

		//���\�[�X�ݒ�
		ResourceProperties(TextureManager::GetInstance().resDesc, sizeIB);
		//�C���f�b�N�X�o�b�t�@�̍쐬
		ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &indexBuff);
		//�C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		Directx::GetInstance().result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indicesLine); i++)
		{
			indexMap[i] = indicesLine[i];//�C���f�b�N�X���R�s�[

		}
		//�}�b�s���O������
		indexBuff->Unmap(0, nullptr);

		//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
		ibViewLine.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibViewLine.Format = DXGI_FORMAT_R16_UINT;
		ibViewLine.SizeInBytes = sizeIB;
	}

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(verticesLine[0]) * _countof(verticesLine));

	//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p

	ResourceProperties(TextureManager::GetInstance().resDesc, sizeVB);

	//���_�o�b�t�@�̐���
	BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &vertBuffLine);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbLineView.BufferLocation = vertBuffLine->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbLineView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbLineView.StrideInBytes = sizeof(verticesLine[0]);
}

void Primitive::InitializeSphere()
{
	//���̗p
	{
		// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
		sizeVB = static_cast<UINT>(sizeof(verticesSphere[0]) * (_countof(verticesSphere)));

		//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p

		ResourceProperties(TextureManager::GetInstance().resDesc, sizeVB);

		//���_�o�b�t�@�̐���
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, vertBuffSphere.GetAddressOf());

		// ���_�o�b�t�@�r���[�̍쐬
		// GPU���z�A�h���X
		vbViewSphere.BufferLocation = vertBuffSphere->GetGPUVirtualAddress();
		// ���_�o�b�t�@�̃T�C�Y
		vbViewSphere.SizeInBytes = sizeVB;
		// ���_1���̃f�[�^�T�C�Y
		vbViewSphere.StrideInBytes = sizeof(verticesSphere[0]);


		WorldMat worldMat;
		Vec3 vec = { 0,-1.0f,0 };

		//���_���
		verticesSphere[0] = { {vec.x,vec.y,vec.z},{},{1.0f,0.0f} };//��
		verticesSphere[1] = { {vec.x,-vec.y,vec.z},{},{1.0f,0.0f} };//��

		for (int i = 0; i < 36; i++)//��
		{
			worldMat.rot.y = (float)i * AngletoRadi(360.0f / 35.0f);


			for (int j = 0; j < 34; j++)//�c
			{
				worldMat.rot.x = ((float)(j + 1) * (pi / 35.0f));
				worldMat.SetWorld();
				vec = { 0,-1.0f,0 };
				Vec3xM4(vec, worldMat.matWorld, false);

				int p = i * 34 + j;
				verticesSphere[(2) + i * 34 + j] = { {vec.x,vec.y,vec.z},{},{1.0f,0.0f} };
			}
		}

		int count = 0;
		int count2 = 0;
		int count3 = 0;
		int count4 = 0;
		bool isLast = false;
		//�C���f�b�N�X
		for (int i = 0; i < _countof(indicesSphere); i++)
		{
			if (i % (PAPA * 35) == 0 && i != 0)//�Ō�̏c�̗�
			{
				isLast = true;
			}

			if (i % PAPA == 0 || i % (PAPA * (count + 1) - 3) == 0)
			{

			}
			else if (1)//��ԉ����ザ��Ȃ����
			{
				if (count2 % 2 == 0)
				{
					if (isLast)
					{
						indicesSphere[i] = 2 + 34 * count + (count3);
						indicesSphere[i + 1] = 2 + 0 + (count3);//������Ă����̂ň�ԍŏ��̗���g��
						indicesSphere[i + 2] = 2 + 34 * count + (count3 + 1);

						count3++;
						i += 2;
					}
					else
					{
						indicesSphere[i] = 2 + 34 * count + (count3);
						indicesSphere[i + 1] = 2 + 34 * (count + 1) + (count3);
						indicesSphere[i + 2] = 2 + 34 * count + (count3 + 1);

						count3++;
						i += 2;
					}
				}
				else if (count2 % 2 == 1)
				{
					if (isLast)
					{
						indicesSphere[i] = 2 + 0 + (count4 + 1);//������Ă����̂ň�ԍŏ��̗���g��
						indicesSphere[i + 1] = 2 + 34 * count + (count4 + 1);
						indicesSphere[i + 2] = 2 + 0 + (count4);//������Ă����̂ň�ԍŏ��̗���g��

						count4++;
						i += 2;
					}
					else
					{
						indicesSphere[i] = 2 + 34 * (count + 1) + (count4 + 1);
						indicesSphere[i + 1] = 2 + 34 * count + (count4 + 1);
						indicesSphere[i + 2] = 2 + 34 * (count + 1) + (count4);

						count4++;
						i += 2;
					}
				}

				count2++;
			}
			if (i % PAPA == 0 || i % (PAPA * (count + 1) - 3) == 0)
			{
				if (i % PAPA == 0)//��ԉ��̎O�p�`
				{
					if (isLast)
					{
						indicesSphere[i] = 0;
						indicesSphere[i + 1] = 2 + 0;
						indicesSphere[i + 2] = 2 + 34 * count;

						i += 2;
					}
					else
					{
						indicesSphere[i] = 2 + 34 * (count + 1);
						indicesSphere[i + 1] = 2 + 34 * count;
						indicesSphere[i + 2] = 0;

						i += 2;
					}
				}
				else if (i % (PAPA * (count + 1) - 3) == 0)//��ԏ�̎O�p�`
				{
					if (isLast)
					{
						indicesSphere[i] = 1 + 34 * (count + 1);
						indicesSphere[i + 1] = 35;
						indicesSphere[i + 2] = 1;

						i += 2;

						count++;
						count2 = 0;
						count3 = 0;
						count4 = 0;
					}
					else
					{
						indicesSphere[i] = 1 + 34 * (count + 1);
						indicesSphere[i + 1] = 1 + 34 * (count + 2);
						indicesSphere[i + 2] = 1;

						i += 2;

						count++;
						count2 = 0;
						count3 = 0;
						count4 = 0;
					}
				}
			}
		}

		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicesSphere));

		//���\�[�X�ݒ�
		ResourceProperties(TextureManager::GetInstance().resDesc, sizeIB);
		//�C���f�b�N�X�o�b�t�@�̍쐬
		ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
		BuffProperties(heapProp, TextureManager::GetInstance().resDesc, &indexBuff);
		//�C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		Directx::GetInstance().result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indicesSphere); i++)
		{
			indexMap[i] = indicesSphere[i];//�C���f�b�N�X���R�s�[

		}
		//�}�b�s���O������
		indexBuff->Unmap(0, nullptr);

		//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
		ibViewSphere.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibViewSphere.Format = DXGI_FORMAT_R16_UINT;
		ibViewSphere.SizeInBytes = sizeIB;


		//06_03
		/*if (indexNum == SPHERE)*/
		{
			for (int i = 0; i < _countof(indicesSphere) / 3; i++)
			{//�O�p�`����ƂɌv�Z
				//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
				unsigned short index0 = indicesSphere[i * 3 + 0];
				unsigned short index1 = indicesSphere[i * 3 + 1];
				unsigned short index2 = indicesSphere[i * 3 + 2];
				//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
				XMVECTOR p0 = XMLoadFloat3(&verticesSphere[index0].pos);
				XMVECTOR p1 = XMLoadFloat3(&verticesSphere[index1].pos);
				XMVECTOR p2 = XMLoadFloat3(&verticesSphere[index2].pos);
				//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z
				XMVECTOR v1 = XMVectorSubtract(p1, p0);
				XMVECTOR v2 = XMVectorSubtract(p2, p0);
				//�O�ρi�����ȃx�N�g���j
				XMVECTOR normal = XMVector3Cross(XMVector3Normalize(v1), XMVector3Normalize(v2));
				//���߂��@���𒸓_�f�[�^�ɑ��
				XMStoreFloat3(&verticesSphere[index0].normal, normal);
				XMStoreFloat3(&verticesSphere[index1].normal, normal);
				XMStoreFloat3(&verticesSphere[index2].normal, normal);
			}
		}
	}
}


