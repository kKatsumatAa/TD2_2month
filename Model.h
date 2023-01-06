#pragma once
#include "Sprite.h"
#include "Mesh.h"
#include <unordered_map>


struct Vertex
{
	XMFLOAT3 pos;   //xyz���W
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;    //uv���W
};
//���_�f�[�^


class Model
{
private:
	static const std::string baseDirectory;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

private:
	// ���O
	std::string name;
	// ���b�V���R���e�i
	std::vector<Mesh*> meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;


private:
	//OBJ�t�@�C������3D���f����ǂݍ���
	void LoadFromOBJInternal(const std::string& folderName, const bool smoothing = false, const bool& modelType = false);

	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	void AddMaterial(Material* material);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTextures();

public:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

public://�ÓI�����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���
	//"�t�H���_���̂�"���w�肷���mtl,obj,textuer��ǂ݂��ށi���ׂē������O�ł���΁j
	static Model* LoadFromOBJ(const std::string& folderName, const bool smoothing = false, const bool& modelType = false);

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Model();

	static void StaticInitialize();


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <param name="modelname">�G�b�W�������t���O</param>
	void Initialize(const std::string& foldername, bool smoothing);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">���ߔ��s��R�}���h���X�g</param>
	void Draw(const int indexNum = 1);
};