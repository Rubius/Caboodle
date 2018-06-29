#include "propertiesdelegateeditorsfactory.h"

#include <QComboBox>
#include <QModelIndex>

#include "PropertiesModule/propertiesmodel.h"
#include "PropertiesModule/property.h"
#include "propertiesstyleddelegatelistener.h"

PropertiesDelegateEditorsFactory::PropertiesDelegateEditorsFactory()
{

}

void PropertiesDelegateEditorsFactory::SetFactory(PropertiesDelegateEditorsFactory* factory)
{
    delete currentFactory();
    currentFactory() = factory;
}

const PropertiesDelegateEditorsFactory&PropertiesDelegateEditorsFactory::Instance()
{
    return *currentFactory();
}

QWidget*PropertiesDelegateEditorsFactory::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const
{
    QVariant delegateData = index.data(PropertiesModel::RoleDelegateData);
    QVariant delegateValue = index.data(PropertiesModel::RoleDelegateValue);
    switch (delegateValue.toInt()) {
    case Property::DelegateNamedUInt: {
        QComboBox* result = new QComboBox(parent);
        result->addItems(delegateData.toStringList());
        return result;
    }
    case Property::DelegatePositionXYZ: {

    }
    default:
        break;
    }

    return nullptr;
}

bool PropertiesDelegateEditorsFactory::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    switch (index.data(PropertiesModel::RoleDelegateValue).toInt()) {
    case Property::DelegateNamedUInt:
        if(auto e = qobject_cast<QComboBox*>(editor)) {
            model->setData(index, e->currentIndex());
        }
        return true;
    default:
        break;
    }
    return false;
}

bool PropertiesDelegateEditorsFactory::setEditorData(QWidget* editor, const QModelIndex& index, const QStyledItemDelegate* delegate) const
{
    switch (index.data(PropertiesModel::RoleDelegateValue).toInt()) {
    case Property::DelegateNamedUInt:
        if(auto e = qobject_cast<QComboBox*>(editor)) {
            e->setCurrentIndex(index.data().toInt());
            auto listener = new PropertiesStyledDelegateListener(e,index,delegate);
            QObject::connect(e, SIGNAL(currentIndexChanged(int)), listener, SLOT(onEditorValueChanged()));
        }
        return true;
    default:
        break;
    }
    return false;
}

PropertiesDelegateEditorsFactory*&PropertiesDelegateEditorsFactory::currentFactory()
{
    static PropertiesDelegateEditorsFactory* currentFactory = new PropertiesDelegateEditorsFactory();
    return currentFactory;
}
