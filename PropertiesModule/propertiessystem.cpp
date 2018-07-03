#include "propertiessystem.h"

#include <SharedModule/stack.h>
#include <SharedModule/threads/threadeventshelper.h>

#include "property.h"

static StackPointers<QHash<Name, Property*>>& contexts()
{
    static StackPointers<QHash<Name, Property*>> res(PropertiesSystem::Max);
    return res;
}

void PropertiesSystem::SetValue(const Name& path, const QVariant& value)
{
    auto find = context().find(path);
    Q_ASSERT_X(find != context().end(), "PropertiesSystem::setValue", path.AsString().toLatin1().constData());
    find.value()->SetValue(value);
}

QVariant PropertiesSystem::GetValue(const Name& path)
{
    auto find = context().find(path);
    Q_ASSERT_X(find != context().end(), "PropertiesSystem::getValue", path.AsString().toLatin1().constData());
    return find.value()->getValue();
}

QVariant PropertiesSystem::GetValue(const Name& path, qint32 type)
{
    auto find = contexts()[type]->find(path);
    Q_ASSERT_X(find != contexts()[type]->end(), "PropertiesSystem::getValue", path.AsString().toLatin1().constData());
    return find.value()->getValue();
}

void PropertiesSystem::Clear()
{
    Q_ASSERT(currentType() != Global);
    for(auto property : context()) {
        delete property;
    }
    context().clear();
}

PropertiesSystem::FHandle& PropertiesSystem::Begin(Type type)
{
    Q_ASSERT(type >= 0 && type < Max);
    currentHandle() = defaultHandle();
    currentType() = type;

    return currentHandle();
}

void PropertiesSystem::Begin(ThreadEventsContainer* thread, PropertiesSystem::Type type)
{
    Begin(type) = [thread](const auto& setter){ thread->Asynch(setter); };
}

void PropertiesSystem::End()
{
    currentHandle() = defaultHandle();
    currentType() = Global;
}

void PropertiesSystem::addProperty(const Name& path, Property* property) {

    Q_ASSERT_X(!context().contains(path), "PropertiesSystem::setValue", path.AsString().toLatin1().constData());
    property->Handler() = currentHandle();
    context().insert(path, property);
}

QHash<Name, Property*>& PropertiesSystem::context(quint8 contextIndex)
{
    return *contexts()[contextIndex];
}

QHash<Name, Property*>& PropertiesSystem::context()
{
    return *contexts()[currentType()];
}

PropertiesSystem::FHandle PropertiesSystem::defaultHandle()
{
    return [](const FSetter& s){ s(); };
}

PropertiesSystem::FHandle& PropertiesSystem::currentHandle()
{
    static FHandle res = defaultHandle();
    return res;
}

PropertiesSystem::Type& PropertiesSystem::currentType()
{
    static Type res = Global; return res;
}
