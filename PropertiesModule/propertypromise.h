#ifndef PROPERTYPROMISE_H
#define PROPERTYPROMISE_H

#include "propertiessystem.h"
#include "property.h"

template<class T>
class PropertyPromise
{
    typedef typename T::value_type value_type;
public:
    PropertyPromise(const Name& name, qint32 contextIndex);
    PropertyPromise(const Name& name, const Property::FOnChange& onChange, qint32 contextIndex);

    const value_type& Native() const { return *_getter(); }
    bool IsValid() const { return _isValid(); }

    operator const value_type& () const { return *_getter(); }
    PropertyPromise& operator=(const value_type& value) { _setter(value); return *this; }

private:
    std::function<T* ()> _getter;
    std::function<bool ()> _isValid;
    std::function<void (const value_type&)> _setter;
};

template<typename T>
struct PropertyPromisePrivate
{
    static QVariant ExtractVariant(const T& value) { return value; }
};

template<typename T>
struct PropertyPromisePrivate<T*>
{
    static QVariant ExtractVariant(T* ptr) { return reinterpret_cast<size_t>(ptr); }
};

template<class T>
PropertyPromise<T>::PropertyPromise(const Name& name, qint32 contextIndex)
    : _getter([name, contextIndex, this]{
        auto context = PropertiesSystem::context(contextIndex);
        auto find = context.find(name);
        Q_ASSERT_X(find != context.end(), "PropertiesSystem::getValue", name.AsString().toLatin1().constData());
        auto property = reinterpret_cast<T*>(find.value());
        _getter = [property]{ return property; };
        return property;
    })
    , _isValid([name, contextIndex, this]{
        auto context = PropertiesSystem::context(contextIndex);
        if(context.contains(name)) {
            _isValid = []{ return true; };
        } else {
            return false;
        }
        return true;
    })
    , _setter([name, contextIndex, this](const value_type& value){
        auto context = PropertiesSystem::context(contextIndex);
        auto find = context.find(name);
        Q_ASSERT_X(find != context.end(), "PropertiesSystem::setValue", name.AsString().toLatin1().constData());
        auto property = reinterpret_cast<T*>(find.value());
        _setter = [property](const value_type& value){
            property->SetValue(PropertyPromisePrivate<value_type>::ExtractVariant(value));
        };
        property->SetValue(PropertyPromisePrivate<value_type>::ExtractVariant(value));
    })
{

}

template<class T>
PropertyPromise<T>::PropertyPromise(const Name& name, const Property::FOnChange& onChange, qint32 contextIndex)
    : PropertyPromise(name, contextIndex)
{
    PropertiesSystem::Subscribe(name, onChange);
}

template<class T>
PropertyPromise<T> PropertiesSystem::GetProperty(const Name& path, qint32 type)
{
    return PropertyPromise<T>(path, type);
}

template<class T>
PropertyPromise<T> PropertiesSystem::GetProperty(const Name& path, const FOnChange& onChange, qint32 type)
{
    return PropertyPromise<T>(path, onChange, type);
}

typedef PropertyPromise<IntProperty> IntPropertyPtr;
typedef PropertyPromise<UIntProperty> UIntPropertyPtr;
typedef PropertyPromise<BoolProperty> BoolPropertyPtr;
typedef PropertyPromise<ByteArrayProperty> ByteArrayPropertyPtr;

template<class T> using PointerPropertyPtr = PropertyPromise<PointerProperty<T>>;

#endif // PROPERTYPROMISE_H
