#ifndef PROPERTIESDELEGATEEDITORSFACTORY_H
#define PROPERTIESDELEGATEEDITORSFACTORY_H

class QWidget;
class QStyleOptionViewItem;
class QModelIndex;
class QAbstractItemModel;
class QStyledItemDelegate;

class PropertiesDelegateEditorsFactory
{
public:
    static void SetFactory(PropertiesDelegateEditorsFactory* factory);
    static const PropertiesDelegateEditorsFactory& Instance();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const;
    virtual bool setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    virtual bool setEditorData(QWidget* editor, const QModelIndex& index, const QStyledItemDelegate* delegate) const;

protected:
    PropertiesDelegateEditorsFactory();

private:
    static PropertiesDelegateEditorsFactory*& currentFactory();
};

#endif // PROPERTIESDELEGATEWIDGETSFACTORY_H
