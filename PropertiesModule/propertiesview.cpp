#include "propertiesview.h"

#ifndef NO_WIDGETS_INTERFACE

#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPainter>
#include <QHeaderView>
#include <QMouseEvent>
#include <QProcess>
#include <QAction>

#include "PropertiesModule/propertiessystem.h"
#include "propertiesmodel.h"
#include "SharedGuiModule/decl.h"
#include "widgets/propertiesdelegatefactory.h"
#include "widgets/propertiesstyleddelegatelistener.h"

class PropertiesDelegate : public QStyledItemDelegate
{
    typedef QStyledItemDelegate Super;
public:
    PropertiesDelegate(QObject* parent)
        : Super(parent)
    {}

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        if(index.data(PropertiesModel::RoleHeaderItem).toBool()){
            QRect orect = option.rect;
            painter->setPen(Qt::NoPen);
            QRect rowRect(0,option.rect.y(),option.widget->width(),orect.height());
            QLinearGradient lg(0,rowRect.y(), rowRect.width(),rowRect.y());
            lg.setColorAt(0, 0x567dbc);
            lg.setColorAt(0.7, 0x6ea1f1);
            painter->setBrush(lg);
            if(!index.column())
                painter->drawRect(orect.adjusted(-orect.x(),0,0,0));
            else
                painter->drawRect(orect);

            QStyleOptionViewItem opt = option;
            initStyleOption(&opt, index);


            const QWidget *widget = option.widget;

            if(float(option.rect.x()) / widget->width() < 0.5)
                opt.palette.setColor(QPalette::Text,Qt::white);
            else
                opt.palette.setColor(QPalette::Text,Qt::red);
            if(index.column()) {
                opt.text = "";
            }

            widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
        }
        else
            QStyledItemDelegate::paint(painter,option,index);
    }

    QWidget*createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE {
        QVariant data = index.data(Qt::EditRole);

        if(auto editor = PropertiesDelegateFactory::Instance().CreateEditor(parent, option, index)) {
            return editor;
        }

        switch (data.type()) {
        case QVariant::Bool: {
            QComboBox* result = new QComboBox(parent);
            result->addItems({ tr("false"), tr("true") });
            result->setFocusPolicy(Qt::StrongFocus);
            return result;
        }
        case QVariant::UInt:
        case QVariant::Int: {
            QSpinBox* result = new QSpinBox(parent);
            result->setValue(data.toInt());
            result->setMinimum(index.data(PropertiesModel::RoleMinValue).toInt());
            result->setMaximum(index.data(PropertiesModel::RoleMaxValue).toInt());
            result->setFocusPolicy(Qt::StrongFocus);
            return result;
        }
        case QVariant::Double:
        case QMetaType::Float: {
            QDoubleSpinBox* result = new QDoubleSpinBox(parent);
            result->setValue(data.toDouble());
            result->setMinimum(index.data(PropertiesModel::RoleMinValue).toDouble());
            result->setMaximum(index.data(PropertiesModel::RoleMaxValue).toDouble());
            auto singleStep = (result->maximum() - result->minimum()) / 100.0;
            singleStep = (singleStep > 1.0) ? 1.0 : singleStep;
            result->setSingleStep(singleStep);
            result->setFocusPolicy(Qt::StrongFocus);
            return result;
        }
        default:
            return Super::createEditor(parent, option, index);
        }
    }

    // QAbstractItemDelegate interface
public:
    void setEditorData(QWidget* editor, const QModelIndex& index) const Q_DECL_OVERRIDE
    {
        if(PropertiesDelegateFactory::Instance().SetEditorData(editor, index, this)) {
            return;
        }

        Super::setEditorData(editor, index);
        if(auto e = qobject_cast<QSpinBox*>(editor)) {
            auto listener = new PropertiesStyledDelegateListener(e,index,this);
            connect(e, SIGNAL(valueChanged(int)), listener, SLOT(onEditorValueChanged()));
        }
        else if(auto e = qobject_cast<QDoubleSpinBox*>(editor)) {
            auto listener = new PropertiesStyledDelegateListener(e,index,this);
            connect(e, SIGNAL(valueChanged(double)), listener, SLOT(onEditorValueChanged()));
        } else if(auto e = qobject_cast<QComboBox*>(editor)) {
            auto listener = new PropertiesStyledDelegateListener(e,index,this);
            connect(e, SIGNAL(currentIndexChanged(int)), listener, SLOT(onEditorValueChanged()));
        }
    }

    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const Q_DECL_OVERRIDE
    {
        if(PropertiesDelegateFactory::Instance().SetModelData(editor, model, index)) {
            return;
        }
        Super::setModelData(editor, model, index);
    }

    virtual QString displayText(const QVariant& value, const QLocale& locale) const Q_DECL_OVERRIDE
    {
        QString result;
        if(PropertiesDelegateFactory::Instance().DisplayText(result, value, locale)) {
            return result;
        }
        return Super::displayText(value, locale);
    }
};

static const StringProperty& textEditor(const char* path = nullptr, const char* value = nullptr)
{
    static StringProperty res(path, value);
    return res;
}

PropertiesView::PropertiesView(QWidget* parent, Qt::WindowFlags flags)
    : PropertiesView(PropertiesSystem::Global, parent, flags)
{

}

PropertiesView::PropertiesView(qint32 contextIndex, QWidget* parent, Qt::WindowFlags flags)
    : Super(parent)
{
    textEditor("Common/Text editor", "C:\\Windows\\system32\\notepad.exe");

    setWindowFlags(windowFlags() | flags);
    setItemDelegate(new PropertiesDelegate(this));
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    header()->hide();
    setIndentation(0);
    setAnimated(true);

    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);

    _propertiesModel = new PropertiesModel(contextIndex, this);
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(_propertiesModel);
    setModel(proxy);

    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    auto addAction = [&](const QString& name, const QString& tr){
        QAction* action = new QAction(tr, this);
        this->addAction(action);
        action->setObjectName(name);
        return action;
    };

    _actionOpenWithTextEditor = addAction("OpenWithTextEditor", tr("Open with text editor"));

    setContextMenuPolicy(Qt::ActionsContextMenu);

    QMetaObject::connectSlotsByName(this);
}

void PropertiesView::SetContextIndex(qint32 contextIndex)
{
    _propertiesModel->SetContextIndex(contextIndex);
}

qint32 PropertiesView::GetContextIndex() const
{
    return _propertiesModel->GetContextIndex();
}

void PropertiesView::Save(const QString& fileName)
{
    _propertiesModel->Save(fileName);
}

void PropertiesView::Load(const QString& fileName)
{
    _propertiesModel->Load(fileName);
}

void PropertiesView::showEvent(QShowEvent*)
{
    if(!model()->rowCount()) {
        _propertiesModel->Change([]{});
    }
}

void PropertiesView::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton) {
        _indexUnderCursor = this->indexAt(event->pos());
        validateActionsVisiblity();
    }
    QAbstractItemView::State preState = state();
    QTreeView::mouseReleaseEvent(event);
    if (preState == QAbstractItemView::AnimatingState)
        setState(preState);
}

void PropertiesView::validateActionsVisiblity()
{
    if(_indexUnderCursor.data(PropertiesModel::RoleDelegateValue).toInt() == Property::DelegateFileName) {
        _actionOpenWithTextEditor->setVisible(true);
    }
    else {
        _actionOpenWithTextEditor->setVisible(false);
    }
}

void PropertiesView::on_OpenWithTextEditor_triggered()
{
    QString openFile = _indexUnderCursor.data().toString();

    QStringList arguments { openFile };

    QProcess *process = new QProcess(this);
    connect(process, SIGNAL(finished(int)), process, SLOT(deleteLater()));
    process->start(textEditor(), arguments);    

    qCWarning(LC_SYSTEM) << "Opening" << textEditor() << arguments;
}

#endif
