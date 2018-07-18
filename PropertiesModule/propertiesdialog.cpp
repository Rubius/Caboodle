#include "propertiesdialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>

#include "propertiesview.h"

PropertiesDialog::PropertiesDialog(qint32 contextIndex, QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new PropertiesView(contextIndex, this));
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
