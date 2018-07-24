#include "threadfunction.h"

ThreadFunction::ThreadFunction(const RunFunction& function, const OnFinishFunction& onFinish)
    : _function(function)
    , _onFinish(onFinish)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    start();
}

void ThreadFunction::Async(const ThreadFunction::RunFunction& function)
{
    new ThreadFunction(function, []{});
}

void ThreadFunction::Async(const RunFunction& function, const OnFinishFunction& onFinish)
{
    new ThreadFunction(function, onFinish);
}

void ThreadFunction::run()
{
    _function();
    _onFinish();
}
