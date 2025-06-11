#pragma once
#include <functional>
#include <any>
#include <queue>
#include <thread>
#include <mutex> 
#include <stdint.h>

class Concurrency  
{
public:
    explicit Concurrency(uint32_t amountOfThreads);
    ~Concurrency();

    void Run(const std::function<void()>& job);

    void Destroy();

private:
    std::vector<std::thread> threads;
    
    // related
    std::queue<std::function<void()>> callbacks;
    std::mutex mutex;

    std::atomic_bool isRunning;
    std::condition_variable conditional;

    void Update();

};

