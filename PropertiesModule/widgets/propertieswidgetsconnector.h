#ifndef PROPERTIESWIDGETSCONNECTOR_H
#define PROPERTIESWIDGETSCONNECTOR_H

/*
 *
 * Note: if you delete Connector then property should be deleted also
 * Note: Usually there is no necessity to delete Connector, expecialy if it reffered to Global context index scope
*/

#include "PropertiesModule/propertypromise.h"

class PropertiesConnectorBase;
class QCheckBox;
class QDoubleSpinBox;
class QSpinBox;
class QLineEdit;

class _Export PropertiesConnectorsContainer
{
public:
    PropertiesConnectorsContainer()
    {}

    void AddConnector(PropertiesConnectorBase* connector);
    void Update();
    void Clear();
    bool IsEmpty() const { return _connectors.IsEmpty(); }

private:
    StackPointers<PropertiesConnectorBase> _connectors;
};

class _Export PropertiesConnectorContextIndexGuard
{
    properties_context_index_t _before;
public:
    explicit PropertiesConnectorContextIndexGuard(properties_context_index_t contextIndex);
    explicit PropertiesConnectorContextIndexGuard();
    ~PropertiesConnectorContextIndexGuard();

    template<class T> static PropertyPromise<T> GetProperty(const Name& name)
    {
        return PropertiesSystem::GetProperty<T>(name, currentContextIndex());
    }

private:
    friend class PropertiesConnectorBase;
    static properties_context_index_t& currentContextIndex();
};

class _Export PropertiesConnectorBase : public QObject
{
    typedef std::function<void (const QVariant& )> Setter;   
public:
    PropertiesConnectorBase(const Name& name, const Setter& setter, QWidget* target);
    virtual ~PropertiesConnectorBase();

private:
    void update();

protected:
    friend class PropertiesConnectorsContainer;
    Setter _setter;
    PropertyPtr _propertyPtr;
    QMetaObject::Connection _connection;
    bool _ignorePropertyChange;

protected:
    class PropertyChangeGuard
    {
        bool& _ignorePropertyChange;
    public:
        PropertyChangeGuard(PropertiesConnectorBase* connector);
        ~PropertyChangeGuard();
    };
};

class _Export PropertiesCheckBoxConnector : public PropertiesConnectorBase
{
public:
    PropertiesCheckBoxConnector(const Name& propertyName, QCheckBox* checkBox);
};

class _Export PropertiesLineEditConnector : public PropertiesConnectorBase
{
public:
    PropertiesLineEditConnector(const Name& propertyName, QLineEdit* lineEdit);
};

class _Export PropertiesSpinBoxConnector : public PropertiesConnectorBase
{
public:
    PropertiesSpinBoxConnector(const Name& propertyName, QSpinBox* spinBox);
};

class _Export PropertiesDoubleSpinBoxConnector : public PropertiesConnectorBase
{
public:
    PropertiesDoubleSpinBoxConnector(const Name& propertyName, QDoubleSpinBox* spinBox);
};

class _Export PropertiesTextEditConnector : public PropertiesConnectorBase
{
public:
    PropertiesTextEditConnector(const Name& propertyName, class QTextEdit* textEdit);
};

// Bool property
class _Export PropertiesGroupBoxConnector : public PropertiesConnectorBase
{
public:
    PropertiesGroupBoxConnector(const Name& propertyName, class QGroupBox* groupBox);
};

#endif // PROPERTIESWIDGETSCONNECTOR_H
