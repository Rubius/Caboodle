#ifndef PROCCESSFACTORY_H
#define PROCCESSFACTORY_H

#include <QString>

struct DescProccesValueState
{
    QString Title;
    qint32 Depth;
    bool IsFinished;
    bool IsNextProcessExpected;

    bool IsShouldStayVisible() const { return !IsFinished || IsNextProcessExpected; }
};

class ProcessValue
{
    typedef std::function<bool (ProcessValue*)> FCallback;
    ProcessValue(const FCallback& callback);

    static std::atomic_int& depthCounter();

public:
    virtual ~ProcessValue();

    DescProccesValueState GetState() const { return { GetTitle(), GetDepth(), IsCanceled(), IsSwaping() }; }
    qint32 GetDepth() const { return _valueDepth; }
    const QString& GetTitle() const { return _title; }
    bool IsCanceled() const { return _isCanceled; }
    bool IsSwaping() const { return _isNextProcessExpected; }
    virtual class ProcessDeterminateValue* AsDeterminate() { return nullptr; }

protected:
    void setTitle(const QString& title);
    void cancel();
    void setNextProcessExpected();

    virtual bool step();
    void init(const QString& title);

private:
    friend class ProcessFactory;
    friend class ProcessBase;

    qint32 _valueDepth;
    FCallback _callback;
    QString _title;
    bool _isNextProcessExpected;
    bool _isCanceled;
};

struct DescProcessDeterminateValueState : DescProccesValueState
{
    qint32 CurrentStep;
    qint32 StepsCount;
};

class ProcessDeterminateValue : public ProcessValue
{
    typedef ProcessValue Super;
    using Super::Super;

public:
    ~ProcessDeterminateValue();

    DescProcessDeterminateValueState GetState() const { return { GetTitle(), GetDepth(), IsCanceled(), IsSwaping(), GetCurrentStep(), GetStepsCount() }; }
    qint32 GetCurrentStep() const { return _currentStep; }
    qint32 GetStepsCount() const { return _stepsCount; }
    virtual ProcessDeterminateValue* AsDeterminate() Q_DECL_OVERRIDE{ return this; }

private:
    virtual bool step() Q_DECL_OVERRIDE;

    friend class ProcessBase;
    void init(const QString& title, qint32 stepsCount);
    void increaseStepsCount(qint32 value);

private:
    qint32 _currentStep;
    qint32 _stepsCount;
};

class ProcessFactory
{
    ProcessFactory();
public:
    static ProcessFactory& Instance();

    void SetDeterminateCallback(const ProcessValue::FCallback& options);
    void SetIndeterminateCallback(const ProcessValue::FCallback& options);

private:
    friend class ProcessBase;
    ProcessValue* createIndeterminate() const;
    ProcessDeterminateValue* createDeterminate() const;

private:
    ProcessValue::FCallback _indeterminateOptions;
    ProcessValue::FCallback _determinateOptions;
};

#endif // PROCCESSMANAGER_H
