#ifndef PROPERTY_H
#define PROPERTY_H

#include <QUrl>
#include <functional>

#include <SharedModule/internal.hpp>
#include <SharedGuiModule/decl.h> // Vector3f

class Property {
public:
    typedef std::function<void ()> FSetter;
    typedef std::function<void (const FSetter&)> FHandle;
    typedef std::function<void ()> FOnChange;
    typedef std::function<void (const QVariant& property, QVariant& new_value)> FValidator;

public:
    enum Option {
        Option_IsExportable = 0x1,// if property should be saved or loaded from file
        Option_IsPresentable = 0x2,// if property should be presented in properties model
        Option_IsReadOnly = 0x4, // if property cans be edited from gui

        Options_Default = Option_IsExportable | Option_IsPresentable,
        Options_InternalProperty = 0
    };
    DECL_FLAGS(Options, Option)

    enum DelegateValue {
        DelegateDefault,
        DelegateFileName,
        DelegatePositionXYZ,
        DelegateNamedUInt,

        DelegateUser
    };

    Property(const Name& path);
    bool SetValue(QVariant value);
    const Options& GetOptions() const { return _options; }
    Options& ChangeOptions() { return _options; }

    FHandle& Handler() { return _fHandle; }
    FValidator& Validator() { return _fValidator; }
    FOnChange& OnChange();

    void Subscribe(const FOnChange& onChange);

    void Invoke() { _fOnChange(); }

    virtual DelegateValue GetDelegateValue() const { return DelegateDefault; }
    virtual const QVariant* GetDelegateData() const { return nullptr; }
    virtual void SetDelegateData(const QVariant& value) { SetValue(value); }

    virtual QVariant GetMin() const { return 0; }
    virtual QVariant GetMax() const { return 0; }

protected:
    friend class PropertiesSystem;
    friend class PropertiesModel;

    virtual QVariant getDisplayValue() const { return getValue(); }
    virtual QVariant getValue() const=0;
    virtual void setValueInternal(const QVariant&)=0;

protected:
    FHandle _fHandle;
    FOnChange _fOnChange;
    FValidator _fValidator;
    Options _options;
#ifdef DEBUG_BUILD
    bool _isSubscribed;
#endif
};

template<class T>
class TExternalProperty : public Property
{
protected:
    typedef std::function<T ()> FGetter;
    typedef std::function<void (T value, T oldValue)> FSetter;
public:
    TExternalProperty(const Name& path,const FGetter& getter, const FSetter& setter, const T& min, const T& max)
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

protected:
    FGetter _getter;
    FSetter _setter;
    T _min;
    T _max;
};

class ExternalBoolProperty : public TExternalProperty<bool>
{
public:
    ExternalBoolProperty(const Name& path, const FGetter& getter, const FSetter& setter)
        : TExternalProperty(path, getter, setter, false, true)
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

template<class T>
class TPropertyBase : public Property
{
    typedef TPropertyBase Super;
public:
    typedef T value_type;
    TPropertyBase(const Name& path, const T& initial)
        : Property(path)
        , _value(initial)
    {}

    const T& Native() const { return _value; }
    const T* Ptr() const { return &_value; }
    operator const T&() const { return _value; }

    template<class T2> T2 Cast() const { return (T2)_value; }

protected:
    virtual QVariant getValue() const Q_DECL_OVERRIDE { return _value; }
protected:
    T _value;
};

template<class T>
class TProperty : public TPropertyBase<T>
{
public:
    TProperty(const Name& path, const T& initial, const T& min, const T& max)
        : TPropertyBase<T>(path, initial)
        , _min(min)
        , _max(max)
    {}

    void SetMinMax(const T& min, const T& max)
    {
        _min = min;
        _max = max;
        if(_value < _min || _value > _max) {
            SetValue(_value);
        }
    }

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
    TProperty<bool>(const Name& path, bool initial)
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
    TProperty<QString>(const Name& path, const QString& initial)
        : TPropertyBase<QString>(path, initial)
    {}
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE { this->_value = value.toString(); }
};

class TextFileNameProperty : public TProperty<QString>
{
public:
    TextFileNameProperty(const Name& path, const QString& initial)
        : TProperty<QString>(path, initial)
    {}
    virtual DelegateValue GetDelegateValue() const Q_DECL_OVERRIDE { return DelegateFileName; }
};

template<>
class TProperty<QUrl> : public TPropertyBase<QUrl>
{
public:
    TProperty<QUrl>(const Name& path, const QUrl& initial)
        : TPropertyBase<QUrl>(path, initial)
    {}
protected:
    virtual void setValueInternal(const QVariant& value) Q_DECL_OVERRIDE{ this->_value = value.toUrl(); }
};

class NamedUIntProperty : public TProperty<quint32>
{
    typedef TProperty<quint32> Super;
public:
    NamedUIntProperty(const Name& path, const quint32& initial)
        : Super(path, initial, 0, 0)
    {}

    void SetNames(const QStringList& names);

    virtual DelegateValue GetDelegateValue() const Q_DECL_OVERRIDE { return DelegateNamedUInt; }
    virtual const QVariant* GetDelegateData() const Q_DECL_OVERRIDE{ return &_names; }
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

    Vector3FProperty(const Name& path, const Vector3F& vector);
};

#endif // PROPERTY_H
