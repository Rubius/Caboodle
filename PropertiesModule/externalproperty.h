#ifndef EXTERNALPROPERTY_H
#define EXTERNALPROPERTY_H

#include "property.h"

template<class T>
class TExternalPropertyBase : public Property
{
protected:
    typedef std::function<T ()> FGetter;
    typedef std::function<void (T value, T oldValue)> FSetter;
public:
    TExternalPropertyBase(const Name& path,const FGetter& getter, const FSetter& setter)
        : Property(path)
        , _getter(getter)
        , _setter(setter)
    {}

protected:
    virtual QVariant getValue() const Q_DECL_OVERRIDE { return _getter(); }

protected:
    FGetter _getter;
    FSetter _setter;
};

template<class T>
class TExternalProperty : public TExternalPropertyBase<T>
{
    typedef TExternalPropertyBase<T> Super;
public:
    TExternalProperty(const Name& path,const FGetter& getter, const FSetter& setter, const T& min, const T& max)
        : Super(path, getter, setter)
        , _min(min)
        , _max(max)
    {}

    virtual QVariant GetMin() const { return _min; }
    virtual QVariant GetMax() const { return _max; }

    // Property interface
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { _setter(value.toDouble(), _getter()); }
    T _min;
    T _max;
};

template<>
class TExternalProperty<QString> : public TExternalPropertyBase<QString>
{
    typedef TExternalPropertyBase<QString> Super;
public:
    TExternalProperty(const Name& path,const FGetter& getter, const FSetter& setter)
        : Super(path, getter, setter)
    {}
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { _setter(value.toString(), _getter()); }
};

template<>
class TExternalProperty<QByteArray> : public TExternalPropertyBase<QByteArray>
{
    typedef TExternalPropertyBase<QByteArray> Super;
public:
    TExternalProperty(const Name& path,const FGetter& getter, const FSetter& setter)
        : Super(path, getter, setter)
    {}
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { _setter(value.toByteArray(), _getter()); }
};

template<>
class TExternalProperty<bool> : public TExternalPropertyBase<bool>
{
    typedef TExternalPropertyBase<bool> Super;
public:
    TExternalProperty(const Name& path, const FGetter& getter, const FSetter& setter)
        : Super(path, getter, setter)
    {}

protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { _setter(value.toBool(), _getter()); }
};

class ExternalNamedUIntProperty : public TExternalProperty<quint32>
{
    typedef TExternalProperty<quint32> Super;
public:
    ExternalNamedUIntProperty(const Name& path, const FGetter& getter, const FSetter& setter)
        : Super(path, getter, setter, 0, 0)
    {}

    void SetNames(const QStringList& names);

    virtual DelegateValue GetDelegateValue() const Q_DECL_OVERRIDE { return DelegateNamedUInt; }
    virtual const QVariant* GetDelegateData() const Q_DECL_OVERRIDE{ return &_names; }
protected:
    virtual QVariant getDisplayValue() const Q_DECL_OVERRIDE { return _names.value<QStringList>()[_getter()]; }

private:
    QVariant _names;
};

// Externals
typedef TExternalProperty<bool> ExternalBoolProperty;
typedef TExternalProperty<double> ExternalDoubleProperty;
typedef TExternalProperty<float> ExternalFloatProperty;
typedef TExternalProperty<qint32> ExternalIntProperty;
typedef TExternalProperty<quint32> ExternalUIntProperty;
typedef TExternalProperty<QString> ExternalStringProperty;
typedef TExternalProperty<QUrl> ExternalUrlProperty;
typedef TExternalProperty<QByteArray> ExternalByteArrayProperty;

#endif // EXTERNALPROPERTY_H
