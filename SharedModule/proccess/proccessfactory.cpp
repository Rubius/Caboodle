#include "proccessfactory.h"

ProccessValue::ProccessValue(const FCallback& callback)
    : _valueDepth(depthCounter()++)
    , _callback(callback)
    , _isCanceled(false)
    , _isSwaping(false)
{

}

std::atomic_int& ProccessValue::depthCounter()
{
    static std::atomic_int res;
    return res;
}

ProccessValue::~ProccessValue()
{
    cancel();
    --depthCounter();
}



void ProccessValue::setTitle(const QString& title)
{
    _title = title;
    _callback(this);
}

void ProccessValue::cancel()
{
    if(!_isCanceled) {
        _isCanceled = true;
        _callback(this);
    }
}

void ProccessValue::setSwaping()
{
    _isSwaping = true;
}

bool ProccessValue::step()
{
    return _callback(this);
}

void ProccessValue::init(const QString& title)
{
    _title = title;
    _callback(this);
}

ProccessDeterminateValue::~ProccessDeterminateValue()
{
    cancel();
}

bool ProccessDeterminateValue::step()
{
    _currentStep++;
    return Super::step();
}

void ProccessDeterminateValue::init(const QString& title, qint32 stepsCount)
{
    _currentStep = 0;
    _stepsCount = stepsCount;
    Super::init(title);
}

void ProccessDeterminateValue::increaseStepsCount(qint32 value)
{
    _stepsCount += value;
    (void)Super::step();
}

static bool DoNothingCallback(ProccessValue*) { return true; }

ProccessFactory::ProccessFactory()
    : _indeterminateOptions(&DoNothingCallback)
    , _determinateOptions(&DoNothingCallback)
{

}

ProccessFactory& ProccessFactory::Instance()
{
    static ProccessFactory ret;
    return ret;
}

void ProccessFactory::SetDeterminateCallback(const ProccessValue::FCallback& options)
{
    _determinateOptions = options;
}

void ProccessFactory::SetIndeterminateCallback(const ProccessValue::FCallback& options)
{
    _indeterminateOptions = options;
}

ProccessValue* ProccessFactory::createIndeterminate() const
{
    return new ProccessValue(_indeterminateOptions);
}

ProccessDeterminateValue* ProccessFactory::createDeterminate() const
{
    return new ProccessDeterminateValue(_determinateOptions);
}

