#ifndef PROCCESSFACTORY_H
#define PROCCESSFACTORY_H

#include <QString>

struct DescProccesValueState
{
    QString Title;
    qint32 Depth;
    bool IsFinished;
    bool IsSwaping;

    bool IsShouldStayVisible() const { return !IsFinished || IsSwaping; }
};

class ProccessValue
{
    typedef std::function<bool (ProccessValue*)> FCallback;
    ProccessValue(const FCallback& callback);

    static std::atomic_int& depthCounter();

public:
    virtual ~ProccessValue();

    DescProccesValueState GetState() const { return { GetTitle(), GetDepth(), IsCanceled(), IsSwaping() }; }
    qint32 GetDepth() const { return _valueDepth; }
    const QString& GetTitle() const { return _title; }
    bool IsCanceled() const { return _isCanceled; }
    bool IsSwaping() const { return _isSwaping; }
    virtual class ProccessDeterminateValue* AsDeterminate() { return nullptr; }

protected:
    void setTitle(const QString& title);
    void cancel();
    void setSwaping();

    virtual bool step();
    void init(const QString& title);

private:
    friend class ProccessFactory;
    friend class ProccessBase;

    qint32 _valueDepth;
    FCallback _callback;
    QString _title;
    bool _isSwaping;
    bool _isCanceled;
};

struct DescProccessDeterminateValueState : DescProccesValueState
{
    qint32 CurrentStep;
    qint32 StepsCount;
};

class ProccessDeterminateValue : public ProccessValue
{
    typedef ProccessValue Super;
    using Super::Super;

public:
    ~ProccessDeterminateValue();

    DescProccessDeterminateValueState GetState() const { return { GetTitle(), GetDepth(), IsCanceled(), IsSwaping(), GetCurrentStep(), GetStepsCount() }; }
    qint32 GetCurrentStep() const { return _currentStep; }
    qint32 GetStepsCount() const { return _stepsCount; }
    virtual ProccessDeterminateValue* AsDeterminate() Q_DECL_OVERRIDE{ return this; }

private:
    virtual bool step() Q_DECL_OVERRIDE;

    friend class ProccessBase;
    void init(const QString& title, qint32 stepsCount);
    void increaseStepsCount(qint32 value);

private:
    qint32 _currentStep;
    qint32 _stepsCount;
};

class ProccessFactory
{
    ProccessFactory();
public:
    static ProccessFactory& Instance();

    void SetDeterminateCallback(const ProccessValue::FCallback& options);
    void SetIndeterminateCallback(const ProccessValue::FCallback& options);

private:
    friend class ProccessBase;
    ProccessValue* createIndeterminate() const;
    ProccessDeterminateValue* createDeterminate() const;

private:
    ProccessValue::FCallback _indeterminateOptions;
    ProccessValue::FCallback _determinateOptions;
};

#endif // PROCCESSMANAGER_H
