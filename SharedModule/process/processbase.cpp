#include "processbase.h"

#include "processfactory.h"


ProcessBase::ProcessBase()
{

}

ProcessBase::~ProcessBase()
{

}

void ProcessBase::beginProcess(const wchar_t* title)
{
    if(_processValue != nullptr) {
        _processValue->setNextProcessExpected();
    }
    _processValue = nullptr;
    _processValue.reset(ProcessFactory::Instance().createIndeterminate());
    _processValue->init(title);
}

void ProcessBase::beginProcess(const wchar_t* title, int stepsCount)
{
    if(_processValue != nullptr) {
        _processValue->setNextProcessExpected();
    }
    _processValue = nullptr;
    auto value = ProcessFactory::Instance().createDeterminate();
    value->init(title, stepsCount);
    _processValue.reset(value);
}

void ProcessBase::setProcessTitle(const wchar_t* title)
{
    _processValue->setTitle(title);
}

void ProcessBase::increaseProcessStepsCount(int stepsCount)
{
    if(auto determinate = _processValue->AsDeterminate()) {
        determinate->increaseStepsCount(stepsCount);
    }
}

void ProcessBase::incrementProcess()
{
    _processValue->incrementStep();
}

bool ProcessBase::isProcessCanceled() const
{
    return _processValue->IsCanceled();
}
