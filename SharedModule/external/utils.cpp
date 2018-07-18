#include "utils.h"
#ifndef NO_WIDGETS_INTERFACE
#include <QAction>

QAction* createAction(const QString& title, const std::function<void ()>& handle)
{
    auto result = new QAction(title);
    result->connect(result, &QAction::triggered, handle);
    return result;
}

void forEachModelIndex(QAbstractItemModel* model, QModelIndex parent, const std::function<void (const QModelIndex&)>& function)
{
    for(int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        function(index);
        // here is your applicable code
        if( model->hasChildren(index) ) {
            forEachModelIndex(model, index, function);
        }
    }
}
#endif
