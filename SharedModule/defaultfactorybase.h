#ifndef DEFAULTFACTORYBASE_H
#define DEFAULTFACTORYBASE_H

#include <QHash>
#include <functional>

template<class ObjectBase>
class DefaultFactoryBase
{
    typedef std::function<ObjectBase* ()> ImporterCreator;
    typedef QHash<QString, ImporterCreator> Delegates;

public:
    static ObjectBase* Create(const QString& extension);

    static bool IsSupport(const QString& extension) { return delegates().contains(extension); }

protected:
    static void assosiate(const QString& formats, const ImporterCreator& importerCreator);

private:
    static Delegates& delegates()
    {
        static Delegates ret;
        return ret;
    }
};

template<class ObjectBase>
ObjectBase* DefaultFactoryBase<ObjectBase>::Create(const QString& fileExtension)
{
    auto find = delegates().find(fileExtension.toLower());
    if(find == delegates().end()) {
         return nullptr;
    }
    return find.value()();
}

template<class ObjectBase>
void DefaultFactoryBase<ObjectBase>::assosiate(const QString& formats, const ImporterCreator& importerCreator)
{
    for(const auto& format : formats.split(" ", QString::SkipEmptyParts)) {
        delegates().insert(format, importerCreator);
    }
}

#endif // DEFAULTFACTORYBASE_H
