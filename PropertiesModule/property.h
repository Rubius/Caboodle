#ifndef PROPERTY_H
#define PROPERTY_H

#include <QUrl>
#include <functional>

#include <SharedModule/decl.h>
#include <SharedGuiModule/decl.h> // Vector3f


class Property {
public:
    typedef std::function<void ()> FSetter;
    typedef std::function<void (const FSetter&)> FHandle;
    typedef std::function<void ()> FOnChange;
    typedef std::function<void (const QVariant& property, QVariant& new_value)> FValidator;

public:
    enum DelegateValue {
        DelegateDefault,
        DelegateFileName,
        DelegatePositionXYZ,
        DelegateNamedUInt,

        DelegateUser
    };

    Property(const QString& path);
    void SetValue(QVariant value);

    FHandle& Handler() { return _fHandle; }
    FValidator& Validator() { return _fValidator; }
    FOnChange& OnChange() { return _fOnset; }

    void Invoke() { _fHandle([]{}); }

    virtual DelegateValue GetDelegateValue() const { return DelegateDefault; }
    virtual const QVariant* GetDelegateData() const { return nullptr; }
    virtual void SetDelegateData(const QVariant& value) { SetValue(value); }

    void SetReadOnly(bool flag) { _bReadOnly = flag; }
    bool IsReadOnly() const { return _bReadOnly; }
    virtual QVariant GetMin() const { return 0; }
    virtual QVariant GetMax() const { return 0; }

protected:
    friend class PropertiesSystem;
    friend class PropertiesModel;

    virtual QVariant getValue() const=0;
    virtual void setValueInternal(const QVariant&)=0;

protected:
    FHandle _fHandle;
    FOnChange _fOnset;
    FValidator _fValidator;
    bool _bReadOnly;
};

template<class T>
class TExternalProperty : public Property
{
    typedef std::function<T ()> FGetter;
    typedef std::function<void (T value, T oldValue)> FSetter;
public:
    TExternalProperty(const QString& path,const FGetter& getter, const FSetter& setter, const T& min, const T& max)
        : Property(path)
        , _getter(getter)
        , _setter(setter)
        , _min(min)
        , _max(max)
    {}

    virtual QVariant GetMin() const { return _min; }
    virtual QVariant GetMax() const { return _max; }

    // Property interface
protected:
    virtual QVariant getValue() const Q_DECL_OVERRIDE { return _getter(); }
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { _setter(value.toDouble(), _getter()); }

private:
    FGetter _getter;
    FSetter _setter;
    T _min;
    T _max;
};

template<class T>
class TPropertyBase : public Property
{
    typedef TPropertyBase Super;
public:
    TPropertyBase(const QString& path, const T& initial)
        : Property(path)
        , _value(initial)
    {}

    T& native() { return _value; }
    T* ptr() { return &_value; }
    operator const T&() const { return _value; }
    operator T&() { return _value; }

protected:
    virtual QVariant getValue() const Q_DECL_OVERRIDE { return _value; }
protected:
    T _value;
};

template<class T>
class TProperty : public TPropertyBase<T>
{
public:
    TProperty(const QString& path, const T& initial, const T& min, const T& max)
        : TPropertyBase<T>(path, initial)
        , _min(min)
        , _max(max)
    {}
    virtual QVariant GetMin() const Q_DECL_OVERRIDE { return _min; }
    virtual QVariant GetMax() const Q_DECL_OVERRIDE { return _max; }
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { this->_value = clamp((T)value.toDouble(), _min, _max); }
protected:
    T _min;
    T _max;
};

template<>
class TProperty<bool> : public TPropertyBase<bool>
{
public:
    TProperty<bool>(const QString& path, bool initial)
        : TPropertyBase<bool>(path, initial)
    {}

    bool& operator=(bool value) { this->_value = value; return this->_value; }
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { this->_value = value.toBool(); }
};

template<>
class TProperty<QString> : public TPropertyBase<QString>
{
public:
    TProperty<QString>(const QString& path, const QString& initial)
        : TPropertyBase<QString>(path, initial)
    {}
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { this->_value = value.toString(); }
};

class TextFileNameProperty : public TProperty<QString>
{
public:
    TextFileNameProperty(const QString& path, const QString& initial)
        : TProperty<QString>(path, initial)
    {}
    virtual DelegateValue GetDelegateValue() const Q_DECL_OVERRIDE { return DelegateFileName; }
};

template<>
class TProperty<QUrl> : public TPropertyBase<QUrl>
{
public:
    TProperty<QUrl>(const QString& path, const QUrl& initial)
        : TPropertyBase<QUrl>(path, initial)
    {}
protected:
    virtual void setValueInternal(const QVariant& value) { this->_value = value.toUrl(); }
};

class NamedUIntProperty : public TProperty<quint32>
{
    typedef TProperty<quint32> Super;
public:
    NamedUIntProperty(const QString& path, const quint32& initial)
        : Super(path, initial, 0, 0)
    {}

    void SetNames(const QStringList& names);

    virtual DelegateValue GetDelegateValue() const Q_DECL_OVERRIDE { return DelegateNamedUInt; }
    virtual const QVariant* GetDelegateData() const { return &_names; }
private:
    QVariant _names;
};

// Internals
typedef TProperty<bool> BoolProperty;
typedef TProperty<double> DoubleProperty;
typedef TProperty<float> FloatProperty;
typedef TProperty<qint32> IntProperty;
typedef TProperty<quint32> UIntProperty;
typedef TProperty<QString> StringProperty;
typedef TProperty<QUrl> UrlProperty;

// Externals
typedef TExternalProperty<bool> ExternalBoolProperty;
typedef TExternalProperty<double> ExternalDoubleProperty;
typedef TExternalProperty<float> ExternalFloatProperty;
typedef TExternalProperty<qint32> ExternalIntProperty;
typedef TExternalProperty<quint32> ExternalUIntProperty;

class Vector3FProperty
{
public:
    FloatProperty X;
    FloatProperty Y;
    FloatProperty Z;

    Vector3FProperty(const QString& path, const Vector3F& vector);
};

#endif // PROPERTY_H
