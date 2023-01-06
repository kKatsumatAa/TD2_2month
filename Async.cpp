#include "Async.h"

void Async::SetLockFlag(bool _)
{
    std::lock_guard<std::mutex>  lock(isLoadedMutex);
    isLoaded = _;
}

bool Async::GetLockFlag()
{
    std::lock_guard<std::mutex>  lock(isLoadedMutex);
    return isLoaded;
}

void Async::AsyncLoad(std::function<void()> p)
{
    //�֐�
    p();

    SetLockFlag(true);
}

void Async::StartAsyncFunction(std::function<void()> p)
{
    //�񓯊������X�^�[�g
    th1 = std::thread([=] { AsyncLoad(p); });
}

void Async::EndThread()
{
    //�I�������
    th1.join();
}
