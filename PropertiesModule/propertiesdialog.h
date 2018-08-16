#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

#include "PropertiesModule/internal.hpp"

class _Export PropertiesDialog : public QDialog
{
    typedef QDialog Super;
    typedef std::function<void ()> StdHandle;
public:
    PropertiesDialog(qint32 contextIndex, QWidget* parent = nullptr, Qt::WindowFlags flags= Qt::WindowFlags());

    void Initialize(const StdHandle& propertiesInitializeFunction, const StdHandle& onChange, bool isReadOnly);
    bool IsDirty() const { return _isDirty; }

    // QDialog interface
public slots:
    virtual void done(int) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

private:
    bool _isDirty;
    class PropertiesView* _propertiesView;
    ByteArrayPropertyPtr _savedGeometry;
};

#endif // PROPERTIESDIALOG_H
