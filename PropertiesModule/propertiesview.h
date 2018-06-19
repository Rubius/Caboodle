#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#ifndef NO_WIDGETS_INTERFACE

#include <QTreeView>
#include "property.h"

class PropertiesView : public QTreeView
{
    Q_OBJECT
public:
    PropertiesView(QWidget* parent=0, Qt::WindowFlags flags=0);

    void Save(const QString& fileName);
    void Load(const QString& fileName);

    // QWidget interface
protected:
    void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void validateActionsVisiblity();
private Q_SLOTS:
    void on_OpenWithTextEditor_triggered();

private:
    typedef QTreeView Super;
    class PropertiesModel* _propertiesModel;
    QModelIndex _indexUnderCursor;

    QAction* _actionOpenWithTextEditor;
};

#endif

#endif // PROPERTIESVIEW_H
