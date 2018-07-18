#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

class PropertiesDialog : public QDialog
{
public:
    PropertiesDialog(qint32 contextIndex, QWidget* parent = nullptr, Qt::WindowFlags flags= Qt::WindowFlags());
};

#endif // PROPERTIESDIALOG_H
