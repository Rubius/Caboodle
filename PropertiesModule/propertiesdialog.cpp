#include "propertiesdialog.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QDialogButtonBox>

#include "propertiesmodel.h"
#include "propertiessystem.h"
#include "propertiesview.h"

PropertiesDialog::PropertiesDialog(qint32 contextIndex, QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , _propertiesView(new PropertiesView(contextIndex, this))
    , _savedGeometry(Name("PropertiesDialogGeometry/" + QString::number(contextIndex)), PropertiesSystem::Global)
{
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(_propertiesView);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void PropertiesDialog::Initialize(const StdHandle& propertiesInitializeFunction, const StdHandle& onChange, bool isReadOnly)
{
    qint32 contextIndex = _propertiesView->GetContextIndex();
    if(isVisible()) {
        qApp->setActiveWindow(this);
        return;
    }

    _isDirty = false;
    _propertiesView->GetPropertiesModel()->Change([isReadOnly, this, contextIndex, propertiesInitializeFunction, onChange]{
        PropertiesSystem::Begin(contextIndex);
        propertiesInitializeFunction();
        if(isReadOnly) {
            PropertiesSystem::ForeachProperty([](Property* property){
                property->ChangeOptions().AddFlag(Property::Option_IsReadOnly);
            });
        } else {
            PropertiesSystem::Subscribe([this, onChange] {
                onChange();
                _isDirty = true;
            });
        }
        PropertiesSystem::End();
    });

    show();
}

void PropertiesDialog::done(int result)
{
    if(_savedGeometry.IsValid()) {
        _savedGeometry = saveGeometry();
    }
    Super::done(result);
    PropertiesSystem::Clear(_propertiesView->GetContextIndex());
}

void PropertiesDialog::showEvent(QShowEvent* event)
{
    if(_savedGeometry.IsValid() && !_savedGeometry.Native().isEmpty()) {
        restoreGeometry(_savedGeometry);
    } else {
        Super::showEvent(event);
    }
}
