#include "CartModel.h"
#include "CartList.h"
#include "MenuData.h"
#include "MenuList.h"

namespace restbes {

CartModel::CartModel(QObject *parent)
        : QAbstractListModel(parent), cartList(nullptr), menuList(nullptr) {
}

int CartModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !cartList || !menuList)
        return 0;

    return cartList->size();
}

QVariant CartModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !cartList || !menuList)
        return {};

    int id = cartList->getId(index.row());
    MenuItem menuItem = menuList->getItem(id);
    switch (role) {
        case IdRole:
            return {id};
        case NameRole:
            return {menuItem.name};
        case ImageRole:
            return {menuItem.image};
        case PriceRole:
            return {menuItem.price};
        case InfoRole:
            return {menuItem.info};
        case StatusRole:
            return {menuItem.status};
        case CountRole:
            return {cartList->getItemAt(index.row()).count};
        default:
            return {};
    }
}

bool
CartModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!cartList || !menuList)
        return false;

    CartItem cartItem = cartList->getItemAt(index.row());
    if (role == CountRole &&
        cartList->setItemCount(cartItem.item_id, value.toInt())) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    } else return false;
}

Qt::ItemFlags CartModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> CartModel::roleNames() const {
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

CartList *CartModel::getCartList() const {
    return cartList;
}

MenuList *CartModel::getMenuList() const {
    return menuList;
}

void CartModel::setCartList(CartList *cList) {
    beginResetModel();

    if (cartList)
        cartList->disconnect(this);

    cartList = cList;

    if (cartList) {
        connect(cartList,
                &CartList::beginInsertItem,
                this,
                [=](int index) {
                    beginInsertRows(QModelIndex(), index, index);
                });
        connect(cartList,
                &CartList::endInsertItem,
                this,
                [=]() {
                    endInsertRows();
                });

        connect(cartList,
                &CartList::beginRemoveItem,
                this,
                [=](int index) {
                    beginRemoveRows(QModelIndex(), index, index);
                });
        connect(cartList,
                &CartList::endRemoveItem,
                this,
                [=]() {
                    endRemoveRows();
                });

        connect(cartList,
                &CartList::beginChangeLayout,
                this,
                [=]() {
                    layoutAboutToBeChanged();
                });
        connect(cartList,
                &CartList::endChangeLayout,
                this,
                [=]() {
                    layoutChanged();
                });
    }

    endResetModel();
}

void CartModel::setMenuList(MenuList *mList) {
    beginResetModel();

    if (menuList)
        menuList->disconnect(this);

    menuList = mList;

    if (menuList) {
        connect(menuList,
                &MenuList::itemChanged,
                this,
                [=](int id) {
                    if (!cartList) return;
                    auto ind = index(cartList->getIndex(id), 0);
                    QVector<int> roles {
                            IdRole,
                            NameRole,
                            ImageRole,
                            PriceRole,
                            InfoRole,
                            StatusRole
                    };
                    dataChanged(ind, ind, roles);
                });
    }

    endResetModel();
}

void CartModel::updatePersistentIndexList(const QModelIndexList &newList) {
    changePersistentIndexList(persistentIndexList(), newList);
}

}
