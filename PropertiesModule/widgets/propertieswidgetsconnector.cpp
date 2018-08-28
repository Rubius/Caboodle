#include "propertieswidgetsconnector.h"

PropertiesConnectorContextIndexGuard::PropertiesConnectorContextIndexGuard(properties_context_index_t contextIndex)
    : _before(currentContextIndex())
{
    currentContextIndex() = contextIndex;
}

PropertiesConnectorContextIndexGuard::PropertiesConnectorContextIndexGuard()
    : _before(currentContextIndex())
{
    currentContextIndex() = PropertiesSystem::GetCurrentContextIndex();
}

PropertiesConnectorContextIndexGuard::~PropertiesConnectorContextIndexGuard()
{
    currentContextIndex() = _before;
}

properties_context_index_t& PropertiesConnectorContextIndexGuard::currentContextIndex()
{
    static properties_context_index_t ret = PropertiesSystem::Global;
    return ret;
}

PropertiesConnectorBase::PropertiesConnectorBase(const Name& name, const PropertiesConnectorBase::Setter& setter, QObject* target)
    : QObject(target)
    , _setter(setter)
    , _propertyPtr(name, [this, target]{
    QSignalBlocker blocker(target);
    _setter(_propertyPtr.GetProperty()->GetValue());
}, PropertiesConnectorContextIndexGuard::currentContextIndex())
{
    if(_propertyPtr.IsValid()) {
        QSignalBlocker blocker(target);
        _setter(_propertyPtr.GetProperty()->GetValue());
    }
}

PropertiesConnectorBase::~PropertiesConnectorBase()
{
    disconnect(_connection);
}

void PropertiesConnectorBase::update()
{
    QSignalBlocker blocker(parent());
    _setter(_propertyPtr.GetProperty()->GetValue());
}

void PropertiesConnectorsContainer::AddConnector(PropertiesConnectorBase* connector)
{
    _connectors.Append(connector);
}

void PropertiesConnectorsContainer::Update()
{
    for(auto connector : _connectors) {
        connector->update();
    }
}

void PropertiesConnectorsContainer::Clear()
{
    _connectors.Clear();
}

