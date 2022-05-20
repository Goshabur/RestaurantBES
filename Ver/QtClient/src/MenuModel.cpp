#include "MenuModel.h"
#include "MenuList.h"
#include "MenuData.h"
#include "CartList.h"

namespace restbes {

MenuModel::MenuModel(QObject *parent)
        : QAbstractListModel(parent), menuList(nullptr), cartList(nullptr) {
}

int MenuModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !menuList || !cartList)
        return 0;

    return menuList->size();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !menuList || !cartList)
        return {};

    const MenuItem item = menuList->getItemAt(index.row());
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
        case CountRole:
            return {cartList->getItemCount(item.id)};
        default:
            return {};
    }
}

bool
MenuModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!menuList || !cartList)
        return false;

    MenuItem item = menuList->getItemAt(index.row());
    switch (role) {
        case CountRole:
            return cartList->setItemCount(item.id, value.toInt());
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

    if (menuList->setItemAt(index.row(), item)) {
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
            {StatusRole, "status"},
            {CountRole,  "count"}
    };
    return names;
}

MenuList *MenuModel::getMenuList() const {
    return menuList;
}

CartList *MenuModel::getCartList() const {
    return cartList;
}

void MenuModel::setMenuList(MenuList *mList) {
    beginResetModel();

    if (menuList)
        menuList->disconnect(this);

    menuList = mList;

    if (menuList) {
        connect(menuList, &MenuList::beginInsertItem, this, [=](int index) {
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(menuList, &MenuList::endInsertItem, this, [=]() {
            endInsertRows();
        });

        connect(menuList, &MenuList::beginRemoveItem, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(menuList, &MenuList::endRemoveItem, this, [=]() {
            endRemoveRows();
        });

        connect(menuList, &MenuList::beginChangeLayout, this, [=]() {
            layoutAboutToBeChanged();
        });
        connect(menuList, &MenuList::endChangeLayout, this, [=]() {
            layoutChanged();
        });
    }

    endResetModel();
}

void MenuModel::setCartList(CartList *cList) {
    beginResetModel();

    if (cartList)
        cartList->disconnect(this);

    cartList = cList;

    if (cartList) {
        connect(cartList, &CartList::itemCountChanged, this,
                [=](int id) {
                    if (!menuList) return;
                    int ind = menuList->getIndex(id);
                    dataChanged(index(ind, 0), index(ind, 0),
                                QVector<int>() << CountRole);
                });
        connect(cartList, &CartList::endChangeLayout, this,
                [=]() {
                    dataChanged(index(0, 0), index(rowCount() - 1, 0),
                                QVector<int>() << CountRole);
                });
    }

    endResetModel();
}

void MenuModel::updatePersistentIndexList(const QModelIndexList &newList) {
    changePersistentIndexList(persistentIndexList(), newList);
}

}
