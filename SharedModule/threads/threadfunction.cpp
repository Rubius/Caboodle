#include "threadfunction.h"

ThreadFunction::ThreadFunction(const RunFunction& function)
    : _function(function)
{
    start();
}

void ThreadFunction::Async(const RunFunction& function)
{
    new ThreadFunction(function);
}

void ThreadFunction::run()
{
    _function();
    deleteLater();
}
