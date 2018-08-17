#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#ifndef NO_WIDGETS_INTERFACE

#include <QTreeView>
#include "property.h"

class PropertiesView : public QTreeView
{
    Q_OBJECT
    typedef QTreeView Super;
    Q_PROPERTY(QColor gradientLeft READ getLeftGradientColor WRITE setLeftGradientColor) TODO. Not compiling in cmake
    Q_PROPERTY(QColor gradientRight READ getRightGradientColor WRITE setRightGradientColor)
    Q_PROPERTY(double gradientRightBorder READ getRightGradientBorder WRITE setRightGradientBorder)
public:
    _Export PropertiesView(QWidget* parent=0, Qt::WindowFlags flags=0);
    _Export PropertiesView(qint32 contextIndex, QWidget* parent=0, Qt::WindowFlags flags=0);

    _Export void SetContextIndex(qint32 contextIndex);
    _Export qint32 GetContextIndex() const;

    _Export void Save(const QString& fileName);
    _Export void Load(const QString& fileName);

    class PropertiesModel* GetPropertiesModel() const { return _propertiesModel; }

    // QWidget interface
protected:
    _Export void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;
    _Export void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void validateActionsVisiblity();

private Q_SLOTS:
    void on_OpenWithTextEditor_triggered();

protected:
    class PropertiesModel* _propertiesModel;

private:
    QModelIndex _indexUnderCursor;
    QAction* _actionOpenWithTextEditor;

private:
    _Export void setLeftGradientColor(const QColor& color);
    _Export void setRightGradientColor(const QColor& color);
    _Export void setRightGradientBorder(double border);

    _Export const QColor& getLeftGradientColor() const;
    _Export const QColor& getRightGradientColor() const;
    _Export double getRightGradientBorder() const;

    class PropertiesDelegate* propertiesDelegate() const { return reinterpret_cast<PropertiesDelegate*>(itemDelegate()); }
};

#endif

#endif // PROPERTIESVIEW_H
