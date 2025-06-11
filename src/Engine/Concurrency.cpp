#include "Concurrency.h"

Concurrency::Concurrency(uint32_t amountOfThreads)
    : isRunning(true)
{
    threads.reserve(amountOfThreads);
    for(uint32_t i = 0; i < amountOfThreads; i++)
    {
        threads.emplace_back(&Concurrency::Update, this);
    }
}

Concurrency::~Concurrency()
{
    Destroy();
}

void Concurrency::Run(const std::function<void()> &job)
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        callbacks.emplace(std::move(job));
    }
    conditional.notify_one();
}

void Concurrency::Destroy()
{
    isRunning = false;
    conditional.notify_all(); // Release all 
    for (std::thread& thread : threads) 
    {
        if (thread.joinable()) 
        {
            thread.join();
        }
    }
}

void Concurrency::Update()
{
    while(isRunning)
    {
        std::function<void()> callback;

        {
            std::unique_lock<std::mutex> lock(mutex);

            conditional.wait(lock, [this] { return !callbacks.empty() || !isRunning; });

            if(!isRunning || callbacks.empty())
                    continue;

            callback = std::move(callbacks.front());  // move locally
            callbacks.pop(); // Remove 
        }

        callback(); // execute
    }
}