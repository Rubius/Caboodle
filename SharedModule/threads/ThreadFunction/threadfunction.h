#ifndef THREADFUNCTION_H
#define THREADFUNCTION_H

#include <functional>

typedef std::function<void ()> FTask;
typedef std::function<void ()> FOnFinish;

class ThreadFunction
{
public:
    static void Async(const FTask& function);
    static void Async(const FTask& function, const FOnFinish& onFinish);

private:
    static class ThreadPool& threadPool();
};

#endif // THREADFUNCTION_H
