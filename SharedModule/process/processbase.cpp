#include "processbase.h"

#include "SharedModule/threads/threadfunction.h"
#include "processfactory.h"


ProcessBase::ProcessBase()
{

}

ProcessBase::~ProcessBase()
{

}

void ProcessBase::beginProcess(const QString& title)
{
    if(_processValue != nullptr) {
        _processValue->setNextProcessExpected();
    }
    _processValue = nullptr;
    _processValue = ProcessFactory::Instance().createIndeterminate();
    _processValue->init(title);
}

void ProcessBase::beginProcess(const QString& title, qint32 stepsCount)
{
    if(_processValue != nullptr) {
        _processValue->setNextProcessExpected();
    }
    _processValue = nullptr;
    auto value = ProcessFactory::Instance().createDeterminate();
    value->init(title, stepsCount);
    _processValue = value;
}

void ProcessBase::setProcessTitle(const QString& title)
{
    _processValue->setTitle(title);
}

void ProcessBase::increaseProcessStepsCount(qint32 stepsCount)
{
    if(auto determinate = _processValue->AsDeterminate()) {
        determinate->increaseStepsCount(stepsCount);
    }
}

bool ProcessBase::stepProcess()
{
    return _processValue->step();
}

void ProcessBase::run()
{
}

void ProcessBase::Start()
{
    ThreadFunction::Async([this]{
        this->run();
        delete this;
    });
}
