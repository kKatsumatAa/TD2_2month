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
	//処理をしてくれる関数
	void AsyncLoad(std::function<void()> p);


public:
	//非同期処理が終わったかどうか
	bool GetLockFlag();

	//フラグをセット
	void SetLockFlag(bool islock);

	//非同期を開始
	void StartAsyncFunction(std::function<void()> p);

	//lockFlagがたったら絶対に実行すること
	void EndThread();
};

