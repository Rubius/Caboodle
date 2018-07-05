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

    operator const value_type& () const { return *_getter(); }

private:
    std::function<T* ()> _getter;
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
{

}

template<class T>
PropertyPromise<T>::PropertyPromise(const Name& name, const Property::FOnChange& onChange, qint32 contextIndex)
    : _getter([name, contextIndex, onChange, this]{
        auto context = PropertiesSystem::context(contextIndex);
        auto find = context.find(name);
        Q_ASSERT_X(find != context.end(), "PropertiesSystem::getValue", name.AsString().toLatin1().constData());
        auto property = reinterpret_cast<T*>(find.value());
        _getter = [property]{ return property; };
        return property;
    })
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

typedef PropertyPromise<UIntProperty> UIntPropertyPtr;

#endif // PROPERTYPROMISE_H
