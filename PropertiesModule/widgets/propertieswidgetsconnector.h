#ifndef PROPERTIESWIDGETSCONNECTOR_H
#define PROPERTIESWIDGETSCONNECTOR_H

/*
 *
 * Note: if you delete Connector then property should be deleted also
 * Note: Usually there is no necessity to delete Connector, expecialy if it reffered to Global context index scope
*/

#include <QCheckBox>

#include "PropertiesModule/propertypromise.h"

class PropertiesConnectorBase;

class PropertiesConnectorsContainer
{
public:
    PropertiesConnectorsContainer()
    {}

    void AddConnector(PropertiesConnectorBase* connector);
    void Update();
    void Clear();

private:
    StackPointers<PropertiesConnectorBase> _connectors;
};

class PropertiesConnectorContextIndexGuard
{
    properties_context_index_t _before;
public:
    explicit PropertiesConnectorContextIndexGuard(properties_context_index_t contextIndex);
    explicit PropertiesConnectorContextIndexGuard();
    ~PropertiesConnectorContextIndexGuard();

private:
    friend class PropertiesConnectorBase;
    static properties_context_index_t& currentContextIndex();
};

class _Export PropertiesConnectorBase : public QObject
{
    typedef std::function<void (const QVariant& )> Setter;
public:
    PropertiesConnectorBase(const Name& name, const Setter& setter, QObject* target);
    virtual ~PropertiesConnectorBase();

private:
    void update();

protected:
    friend class PropertiesConnectorsContainer;
    Setter _setter;
    PropertyPtr _propertyPtr;
    QMetaObject::Connection _connection;
};

class PropertiesCheckBoxConnector : public PropertiesConnectorBase
{
public:
    PropertiesCheckBoxConnector(const Name& propertyName, QCheckBox* checkBox)
        : PropertiesConnectorBase(propertyName,
                                  [checkBox](const QVariant& value){ checkBox->setChecked(value.toBool()); },
                                  checkBox)
    {
        _connection = connect(checkBox, &QCheckBox::clicked, [this](bool value){
            _propertyPtr.GetProperty()->SetValue(value);
        });
    }
};

#include <QDoubleSpinBox>

template<class SpinType, typename valueType>
class PropertiesSpinBoxConnectorBase : public PropertiesConnectorBase
{
public:
    PropertiesSpinBoxConnectorBase(const Name& propertyName, SpinType* spinBox)
        : PropertiesConnectorBase(propertyName,
                                  [spinBox, this](const QVariant& value){
                                    auto property = _propertyPtr.GetProperty();
                                    spinBox->setMinimum(property->GetMin().toDouble());
                                    spinBox->setMaximum(property->GetMax().toDouble());
                                    spinBox->setValue(value.toDouble());
                                    auto singleStep = (spinBox->maximum() - spinBox->minimum()) / 100.0;
                                    singleStep = (singleStep > 1.0) ? 1.0 : singleStep;
                                    spinBox->setSingleStep(singleStep);
                                    spinBox->setFocusPolicy(Qt::StrongFocus);
                                  },
                                  spinBox)
    {
        _connection = connect(spinBox, static_cast<void (SpinType::*)(valueType)>(&SpinType::valueChanged), [this](valueType value){
            _propertyPtr.GetProperty()->SetValue(value);
        });
    }
};

typedef PropertiesSpinBoxConnectorBase<QSpinBox, qint32> PropertiesSpinBoxConnector;
typedef PropertiesSpinBoxConnectorBase<QDoubleSpinBox, double> PropertiesDoubleSpinBoxConnector;


#endif // PROPERTIESWIDGETSCONNECTOR_H
