#include "proccessbase.h"

#include "SharedModule/threads/threadfunction.h"
#include "proccessfactory.h"


ProccessBase::ProccessBase()
{

}

ProccessBase::~ProccessBase()
{

}

void ProccessBase::beginProccess(const QString& title)
{
    if(_proccessValue != nullptr) {
        _proccessValue->setSwaping();
    }
    _proccessValue = nullptr;
    _proccessValue = ProccessFactory::Instance().createIndeterminate();
    _proccessValue->init(title);
}

void ProccessBase::beginProccess(const QString& title, qint32 stepsCount)
{
    if(_proccessValue != nullptr) {
        _proccessValue->setSwaping();
    }
    _proccessValue = nullptr;
    auto value = ProccessFactory::Instance().createDeterminate();
    value->init(title, stepsCount);
    _proccessValue = value;
}

void ProccessBase::setProccessTitle(const QString& title)
{
    _proccessValue->setTitle(title);
}

void ProccessBase::increaseProccessStepsCount(qint32 stepsCount)
{
    if(auto determinate = _proccessValue->AsDeterminate()) {
        determinate->increaseStepsCount(stepsCount);
    }
}

bool ProccessBase::stepProccess()
{
    return _proccessValue->step();
}

void ProccessBase::run()
{
}

void ProccessBase::Start()
{
    ThreadFunction::Async([this]{
        this->run();
        delete this;
    });
}
