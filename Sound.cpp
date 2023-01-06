#include "Sound.h"

IXAudio2MasteringVoice* Sound::masterVoice;
ComPtr<IXAudio2> Sound::xAudio2_;
std::map < std::string, Sound::SoundData> Sound::soundDatas_;
std::string Sound::directoryPath_ = "Resources/sound/";

Sound::Sound()
{
}

Sound::~Sound()
{
	xAudio2_.Reset();

	//�����f�[�^���
	std::map < std::string, Sound::SoundData>::iterator it = soundDatas_.begin();

	for (; it != soundDatas_.end(); ++it)
	{
		//second�̓y�A�̓��
		UnLoad(&it->second);
	}
	soundDatas_.clear();
}

void Sound::Initialize(const std::string& directoryPath_)
{
	Sound::directoryPath_ = directoryPath_;

	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
	}

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
	}
}

void Sound::LoadWave(const std::string& filename, const bool& isConvert)
{
	std::string fullpath = directoryPath_ + filename;

	//�����������[�h���Ă��Ȃ���
	if (soundDatas_.find(fullpath) != soundDatas_.end()) { return; }

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fullpath, std::ios_base::binary);
	assert(file.is_open());

	//RIFF�w�b�_�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//file��Riff���`�F�b�N(�擪�l�������`�F�b�N)
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}
	//���f�[�^��mp3���Ɓ~�I�i�g���q��ς��Ă��_���˃R���o�[�^�ŕϊ��j


//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)//"fmt "�Ō㔼�p�X�y�[�X�I�I�I
	{
		assert(0);
	}

	//�`�����N�{�̂̓ǂݍ���
	if (format.chunk.size > sizeof(format.fmt))//18byte���傫��������
	{
		int32_t size = format.chunk.size - sizeof(format.fmt);
		//18byte���ǂݎ���Ă���
		file.read((char*)&format.fmt, sizeof(format.fmt));

		//����format
		FormatChunk format2 = {};
		//���̍����ǂݎ���Đi�߂�
		file.read((char*)&format2.fmt, size);
	}
	else
	{
		//assert(format.chunk.size <= sizeof(format.fmt));
		file.read((char*)&format.fmt, format.chunk.size);
		//16,18byte�̃t�H�[�}�b�g�`�����N�ɑΉ�
		//�i40byte�́~��40�ǂݍ��񂾌�ŁA�擪18byte�����R�s�[����Ƃ����j
	}

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//junk�`�����N�����o������
	if (strncmp(data.id, "JUNK", 4) != isConvert)//�����ɂ���ĕς�����
	{
		//junk�`�����N�̍Ō�܂œǂݔ�΂�(�ǂݎ��w�b�h�𓮂���)
		file.seekg(data.size, std::ios_base::cur);
		//(�`�����N�w�b�_��)�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	//�{���̃f�[�^�`�����N
	if (strncmp(data.id, "data", 4 != 0))
	{
		assert(0);
	}
	//Data�`�����N�̃f�[�^���i�g�`�f�[�^�̓ǂݍ��݁j
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);


	//wave�t�@�C�������
	file.close();


	//return���邽�߂̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = format.fmt;//�g�`�̃t�H�[�}�b�g
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);//�g�`�f�[�^
	soundData.bufferSize = data.size;//�g�`�f�[�^�̃T�C�Y

	//�T�E���h�f�[�^��A�z�z��Ɋi�[(�������ăZ�b�g�Ń}�b�v�Ɋi�[)
	soundDatas_.insert(std::make_pair(fullpath, soundData));
}

void Sound::UnLoad(SoundData* soundData)
{
	//�o�b�t�@�̃������J��
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Sound::PlayWave(const std::string& filename, const float& volume, const bool& Loop)
{
	HRESULT result;

	std::string fullpath = directoryPath_ + filename;

	//�t�@�C��������T��
	std::map < std::string, Sound::SoundData>::iterator it
		= soundDatas_.find(fullpath);
	//���ǂݍ��݂̌��o
	assert(it != soundDatas_.end());
	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	//���[�v�Đ�
	if (Loop == true) { buf.LoopCount = XAUDIO2_LOOP_INFINITE; }

	//�g�`�f�[�^�̍Đ�&�ݒ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->SetVolume(volume);
	result = pSourceVoice->Start();

	//��~�p
	soundData.pSourceVoice.push_back(pSourceVoice);
}

void Sound::StopWave(const std::string& filename)
{
	std::string fullpath = directoryPath_ + filename;

	//�t�@�C��������T��
	std::map < std::string, Sound::SoundData>::iterator it
		= soundDatas_.find(fullpath);
	//���ǂݍ��݂̌��o
	assert(it != soundDatas_.end());

	//���̃t�@�C�����������
	SoundData& soundData_ = it->second;

	if (soundData_.pSourceVoice.size() > 0)
	{
		for (IXAudio2SourceVoice* source : soundData_.pSourceVoice)
		{
			//�Đ����̓������O�̂����ׂĎ~�߂�
			if (source != nullptr) { source->Stop(); }
		}

		//vector����ɂ���
 		soundData_.pSourceVoice.clear();
	}
}
Sound& Sound::GetInstance()
{
	static Sound inst; // private �ȃR���X�g���N�^���Ăяo���B
	return inst;
}

//void SetpSourceVoice(SoundData& soundData, IXAudio2SourceVoice* pSourceVoice)
//{
//    soundData.pSourceVoice = pSourceVoice;
//}