#ifndef PROCCESSBASE_H
#define PROCCESSBASE_H

#include <functional>
#include <memory>

#include "iprocess.h"

class ProcessValue;

class ProcessDummy : public IProcess
{
public:
    virtual void BeginProcess(const wchar_t*) final {}
    virtual void BeginProcess(const wchar_t*, int) final {}
    virtual void SetProcessTitle(const wchar_t*) final {}
    virtual void IncreaseProcessStepsCount(int) final {}
    virtual void IncrementProcess() final {}
    virtual bool IsProcessCanceled() const final { return false; }
};

class ProcessBase : public IProcess
{
    typedef std::function<void ()> FOnFinish;

public:
    ProcessBase();
    ~ProcessBase();

    void BeginProcess(const wchar_t* title) final;
    void BeginProcess(const wchar_t* title, int stepsCount) final;
    void SetProcessTitle(const wchar_t* title) final;
    void IncreaseProcessStepsCount(int stepsCount) final;
    void IncrementProcess() final;
    bool IsProcessCanceled() const final;

private:
    std::unique_ptr<ProcessValue> _processValue;
};


#endif // PROCCESSBASE_H
