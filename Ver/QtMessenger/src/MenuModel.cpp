#include "MenuModel.h"

#include "MenuList.h"

namespace restbes {

MenuModel::MenuModel(QObject *parent)
        : QAbstractListModel(parent), mList(nullptr) {
}

int MenuModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !mList)
        return {};

    const MenuItem item = mList->getItemAt(index.row());
    switch (role) {
        case IdRole:
            return {item.id};
        case NameRole:
            return {item.name};
        case ImageRole:
            return {item.image};
        case PriceRole:
            return {item.price};
        case InfoRole:
            return {item.info};
        case StatusRole:
            return {item.status};
        default:
            return {};
    }
}

bool
MenuModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!mList)
        return false;

    MenuItem item = mList->getItemAt(index.row());
    switch (role) {
        case IdRole:
            item.id = value.toInt();
            break;
        case NameRole:
            item.name = value.toString();
            break;
        case ImageRole:
            item.image = value.toString();
            break;
        case PriceRole:
            item.price = value.toInt();
            break;
        case InfoRole:
            item.info = value.toString();
            break;
        case StatusRole:
            item.status = value.toInt();
            break;
        default:
            return false;
    }

    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags MenuModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> MenuModel::roleNames() const {
    static QHash<int, QByteArray> names{
            {IdRole,     "id"},
            {NameRole,   "name"},
            {ImageRole,  "image"},
            {PriceRole,  "price"},
            {InfoRole,   "info"},
            {StatusRole, "status"}
    };
    return names;
}

MenuList *MenuModel::list() const {
    return mList;
}

void MenuModel::setList(MenuList *list) {
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &MenuList::beginInsertItem, this, [=](int index) {
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &MenuList::endInsertItem, this, [=]() {
            endInsertRows();
        });

        connect(mList, &MenuList::beginRemoveItem, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &MenuList::endRemoveItem, this, [=]() {
            endRemoveRows();
        });

        connect(mList, &MenuList::beginChangeLayout, this, [=]() {
            layoutAboutToBeChanged();
        });
        connect(mList, &MenuList::endChangeLayout, this, [=]() {
            layoutChanged();
        });
    }

    endResetModel();
}

void MenuModel::updatePersistentIndexList(const QModelIndexList &newList) {
    changePersistentIndexList(persistentIndexList(), newList);
}

}
