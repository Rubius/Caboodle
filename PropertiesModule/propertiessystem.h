#ifndef PROPS_H
#define PROPS_H

#include <functional>
#include <QHash>
#include <QVariant>

class Property;
class Name;
class StateProperty;
template<class T> class PropertyPromise;

class PropertiesSystem
{
public:
    enum Type {
        Global = 0,
        Max = 0xff
    };
    typedef std::function<void ()> FSetter;
    typedef std::function<void (const FSetter&)> FHandle;
    typedef FSetter FOnChange;

    static void SetValueForceInvoke(const Name &path, const QVariant &value);
    static void SetValue(const Name& path, const QVariant& value);
    static void Subscribe(const Name& path, const FOnChange& function);
    static QVariant GetValue(const Name& path);
    static QVariant GetValue(const Name& path, qint32 type);
    template<class T>
    static PropertyPromise<T> GetProperty(const Name& path, qint32 type = Global);
    template<class T>
    static PropertyPromise<T> GetProperty(const Name& path, const FOnChange& onChange, qint32 type = Global);

    static void Load(const QString& fileName, quint8 contextIndex);
    static void Save(const QString& fileName, quint8 contextIndex);
    // clear current context
    static void Clear();

    // begin current context. Global <= type < Max
    // return FHandle reference. It is property setter by default it just call SetValue()
    // every property created between Begin - End uses setted handle
    // change it for example for thread safety.
    // Example:
    // handle = [threadWherePropertyIs](const auto& setter){ threadWherePropertyIs->Asynch(setter); }
    static FHandle& Begin(Type type=Global);
    static FHandle& Begin(qint32 type) { return Begin((Type)type); }
    // convenient Begin overload. Use it when property exists in different from the main thread
    static void Begin(class ThreadEventsContainer* thread, Type type=Global);
    // call this to
    static void End();

private:
    friend class Property;
    friend class PropertiesModel;
    template<class T> friend class PropertyPromise;

    PropertiesSystem();
    Q_DISABLE_COPY(PropertiesSystem)

    static void addProperty(const Name& path, Property* property);

    static QHash<Name, Property*>& context(quint8 contextIndex);
    static QHash<Name, Property*>& context();
    static FHandle defaultHandle();
    static FHandle& currentHandle();
    static Type& currentType();
};

#endif // PROPS_H
