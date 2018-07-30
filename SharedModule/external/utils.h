#ifndef UTILS_H
#define UTILS_H

#include <functional>

#ifndef NO_GUI

#include <QAbstractItemModel>

void forEachModelIndex(QAbstractItemModel* model, QModelIndex parent, const std::function<void (const QModelIndex& index)>& function);
class QAction* createAction(const QString& title, const std::function<void ()>& handle);
class QAction* createAction(const QString &title, const std::function<void (QAction*)> &handle);

#endif

#endif // UTILS_H
