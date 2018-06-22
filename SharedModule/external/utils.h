#ifndef UTILS_H
#define UTILS_H

#include <QAbstractItemModel>
#include <functional>

static void forEachModelIndex(QAbstractItemModel* model, QModelIndex parent, const std::function<void (const QModelIndex& index)>& function) {
    for(int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        function(index);
        // here is your applicable code
        if( model->hasChildren(index) ) {
            forEachModelIndex(model, index, function);
        }
    }
}

#endif // UTILS_H
