#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<cassert>
#include <fstream>
#include <string>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <map>
#include <vector>


class Sound final
{
public:
	//��
	static IXAudio2MasteringVoice* masterVoice;
	//��
	static ComPtr<IXAudio2> xAudio2_;


public://�T�u�N���X
	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];//�`�����N���Ƃ�ID
		int32_t size;//�`�����N�T�C�Y
	};
	//RIFF�w�b�_
	struct RiffHeader
	{
		ChunkHeader chunk;//"RIFF"
		char type[4];//"WAVE"
	};
	//fmt�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;//"fmt"
		WAVEFORMATEX fmt;//"�g�`�t�H�[�}�b�g"
	};

	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer = nullptr;
		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
		//
		std::vector<IXAudio2SourceVoice*> pSourceVoice = { nullptr };
	};

private:
	//�T�E���h�f�[�^�̘A�z�z��
	static std::map < std::string, SoundData> soundDatas_;
	//�T�E���h�i�[�f�B���N�g��
	static std::string directoryPath_;


private:
	Sound();
	~Sound();

public://�����o�֐�

	static void Initialize(const std::string& directoryPath_ = "Resources/sound/");

	/// <summary>
/// ���̓ǂݍ��݁i��2�͊�{true,�ǂݍ��߂Ȃ�/�Đ�����Ȃ��Ƃ���false�ɂ���j
/// </summary>
/// <param name="filename"></param>
/// <param name="isConvert"></param>
/// <returns></returns>
	static void LoadWave(const std::string& filename, const bool& isConvert);
	/// <summary>
	/// �������
	/// </summary>
	/// <param name="soudData"></param>
	/// <returns></returns>
	static void UnLoad(SoundData* soundData);


	/// <summary>
	/// �����Đ�
	/// </summary>
	/// <param name="">wav�t�@�C����</param>
	/// <param name="volume">����</param>
	/// <param name="Loop">���[�v�Đ�</param>
	static void PlayWave(const std::string& filename, const float& volume = 5.0f, const bool& Loop = false);

	static void StopWave(const std::string& filename);


public://�V���O���g��
	//�R�s�[�R���X�g���N�^�𖳌�
	Sound(const Sound& obj) = delete;
	//������Z�q��
	Sound& operator=(const Sound& obj) = delete;

	//�֐�
	static Sound& GetInstance();
};






