#include "processfactory.h"

ProcessValue::ProcessValue(const FCallback& callback)
    : _valueDepth(depthCounter()++)
    , _callback(callback)
    , _isCanceled(false)
    , _isNextProcessExpected(false)
{

}

std::atomic_int& ProcessValue::depthCounter()
{
    static std::atomic_int res;
    return res;
}

ProcessValue::~ProcessValue()
{
    cancel();
    --depthCounter();
}



void ProcessValue::setTitle(const QString& title)
{
    _title = title;
    _callback(this);
}

void ProcessValue::cancel()
{
    if(!_isCanceled) {
        _isCanceled = true;
        _callback(this);
    }
}

void ProcessValue::setNextProcessExpected()
{
    _isNextProcessExpected = true;
}

bool ProcessValue::step()
{
    return _callback(this);
}

void ProcessValue::init(const QString& title)
{
    _title = title;
    _callback(this);
}

ProcessDeterminateValue::~ProcessDeterminateValue()
{
    cancel();
}

bool ProcessDeterminateValue::step()
{
    _currentStep++;
    return Super::step();
}

void ProcessDeterminateValue::init(const QString& title, qint32 stepsCount)
{
    _currentStep = 0;
    _stepsCount = stepsCount;
    Super::init(title);
}

void ProcessDeterminateValue::increaseStepsCount(qint32 value)
{
    _stepsCount += value;
    (void)Super::step();
}

static bool DoNothingCallback(ProcessValue*) { return true; }

ProcessFactory::ProcessFactory()
    : _indeterminateOptions(&DoNothingCallback)
    , _determinateOptions(&DoNothingCallback)
{

}

ProcessFactory& ProcessFactory::Instance()
{
    static ProcessFactory ret;
    return ret;
}

void ProcessFactory::SetDeterminateCallback(const ProcessValue::FCallback& options)
{
    _determinateOptions = options;
}

void ProcessFactory::SetIndeterminateCallback(const ProcessValue::FCallback& options)
{
    _indeterminateOptions = options;
}

ProcessValue* ProcessFactory::createIndeterminate() const
{
    return new ProcessValue(_indeterminateOptions);
}

ProcessDeterminateValue* ProcessFactory::createDeterminate() const
{
    return new ProcessDeterminateValue(_determinateOptions);
}

