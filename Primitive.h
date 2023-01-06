#pragma once
#include"Model.h"

static const int PAPA = 66 * 3 + 6;


/// <summary>
/// �}�`�p�̏����������N���X�idraw�Ŏg���̂ň�̂ݐ����j
/// </summary>
class Primitive
{
private:

public:
	//���_�o�b�t�@�̐���
	ComPtr < ID3D12Resource> vertBuffTriangle = nullptr;
	ComPtr < ID3D12Resource> vertBuffBox = nullptr;
	ComPtr < ID3D12Resource> vertBuffCircle = nullptr;
	ComPtr < ID3D12Resource> vertBuffCube = nullptr;
	ComPtr < ID3D12Resource> vertBuffLine = nullptr;
	ComPtr<ID3D12Resource> vertBuffSphere = nullptr;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbTriangleView{};
	D3D12_VERTEX_BUFFER_VIEW vbBoxView{};
	D3D12_VERTEX_BUFFER_VIEW vbCircleView{};
	D3D12_VERTEX_BUFFER_VIEW vbCubeView{};
	D3D12_VERTEX_BUFFER_VIEW vbLineView{};
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbViewSphere{};

	//04_01
		//���_�f�[�^�\����

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibViewTriangle{};
	D3D12_INDEX_BUFFER_VIEW ibViewBox{};
	D3D12_INDEX_BUFFER_VIEW ibViewCircle{};
	D3D12_INDEX_BUFFER_VIEW ibViewLine{};
	D3D12_INDEX_BUFFER_VIEW ibViewCube{};
	D3D12_INDEX_BUFFER_VIEW ibViewSphere{};

	//�����Ȑ}�`�p
	Vertex verticesTriangle[3] = {
		//��O
		{{-1.0f,-1.0f,-1.0f},{},{0.0f,1.0f}},//����
		{{-1.0f,1.0f, -1.0f},{},{0.0f,0.0f}},//����
		{{1.0f,-1.0f, -1.0f},{},{1.0f,1.0f}},//�E��
	};
	Vertex verticesBox[4] = {
		//��O
		{{-1.0f,-1.0f,-1.0f},{},{0.0f,1.0f}},//����
		{{-1.0f,1.0f, -1.0f},{},{0.0f,0.0f}},//����
		{{1.0f,-1.0f, -1.0f},{},{1.0f,1.0f}},//�E��
		{{1.0f,1.0f,  -1.0f},{},{1.0f,0.0f}},//�E��
	};
	Vertex verticesCircle[24] = {};
	Vertex verticesCube[24] = {};
	Vertex verticesLine[2] = {};
	//����
	Vertex verticesSphere[2 + 34 * 36];
	
	unsigned short indicesSphere[PAPA * 36];
	

public:
	void Initialize();

	void InitializeTriangle();
	void InitializeBox();
	void InitializeCircle();
	void InitializeCube();
	void InitializeLine();
	void InitializeSphere();

};


static unsigned short indicesBox[6] =
{
	0,1,2,//�O�p�`1��
	2,1,3,//�O�p�`2��
};
static unsigned short indicesTriangle[3] =
{
	0,1,2//�O�p�`2��
};
static unsigned short indicesCube[36] =
{
	//�O
	0,1,2,//�O�p�`1��
	2,1,3,//�O�p�`2��
	//��
	6,5,4,//�O�p�`1��
	7,5,6,//�O�p�`2��
	//��
	10,9,8,//�O�p�`1��
	11,9,10,//�O�p�`2��
	//��
	12,13,14,//�O�p�`1��
	14,13,15,//�O�p�`2��
	//��
	16,17,18,//�O�p�`1��
	18,17,19,//�O�p�`2��
	//�E
	22,21,20,//�O�p�`1��
	23,21,22,//�O�p�`2��
};
static unsigned short indicesCircle[] =
{
	2,1,0,
	3,2,0,
	4,3,0,
	5,4,0,
	6,5,0,
	7,6,0,
	8,7,0,
	9,8,0,
	10,9,0,
	11,10,0,
	12,11,0,
	13,12,0,
	14,13,0,
	15,14,0,
	16,15,0,
	17,16,0,
	18,17,0,
	19,18,0,
	20,19,0,
	21,20,0,
	22,21,0,
	23,22,0,
};

static unsigned short indicesLine[2] =
{
	0,1//�O�p�`2��
};
