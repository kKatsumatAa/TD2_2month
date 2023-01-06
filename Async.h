#pragma once
#include<thread>
#include<mutex>
#include<functional>


class Async
{
private:
	bool isLoaded = false;
	std::mutex isLoadedMutex;
	std::thread th1;


private:
	//���������Ă����֐�
	void AsyncLoad(std::function<void()> p);
	//�t���O���Z�b�g
	void SetLockFlag(bool _);

public:
	//�񓯊��������I��������ǂ���
	bool GetLockFlag();

	//�񓯊����J�n
	void StartAsyncFunction(std::function<void()> p);

	//lockFlag�����������΂Ɏ��s���邱��
	void EndThread();
};

