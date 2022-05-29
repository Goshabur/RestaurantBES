#include "MenuModel.h"
#include "CartList.h"
#include "MenuList.h"
#include "MenuItem.h"

#define REFERENCE_CONCAT(x, y) & x :: y

#define CONNECT_SIGNALS(source, sourceClass) \
connect(source, REFERENCE_CONCAT(sourceClass,beginInsertItem), this, [=](int index) { \
    beginInsertRows(QModelIndex(), index, index); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,endInsertItem), this, [=]() { \
    endInsertRows(); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,beginRemoveItem), this, [=](int index) { \
    beginRemoveRows(QModelIndex(), index, index); \
});                                         \
connect(source, REFERENCE_CONCAT(sourceClass,endRemoveItem), this, [=]() { \
    endRemoveRows(); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,beginChangeLayout), this, [=]() { \
    layoutAboutToBeChanged(); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,endChangeLayout),this, [=]() {           \
    updatePersistentIndexList();                                         \
    layoutChanged(); \
})


namespace restbes {

MenuModel::MenuModel(QObject *parent)
        : QAbstractListModel(parent) {
}

int MenuModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !menuList || !cartList)
        return 0;

    if (displayMode == ShowCart) return cartList->size();
    else return menuList->size();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !menuList || !cartList)
        return {};

    int id = (displayMode == ShowCart) ? cartList->getId(index.row())
                                       : menuList->getId(index.row());
    const MenuItem item = menuList->getItem(id);
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

int MenuModel::getDisplayMode() const {
    return displayMode;
}

void MenuModel::setMenuList(MenuList *mList) {
    beginResetModel();

    if (menuList)
        menuList->disconnect(this);

    menuList = mList;

    if (menuList) {
        if (displayMode == ShowMenu) {
            CONNECT_SIGNALS(menuList, MenuList);
        } else {
            connect(menuList,
                    &MenuList::itemChanged,
                    this,
                    [=](int id) {
                        auto ind = index(cartList->getIndex(id), 0);
                        QVector<int> roles{
                                IdRole,
                                NameRole,
                                ImageRole,
                                PriceRole,
                                InfoRole,
                                StatusRole
                        };
                        dataChanged(ind, ind, roles);
                    });
            connect(menuList,
                    &MenuList::endChangeLayout,
                    this,
                    [=]() {
                        QVector<int> roles{
                                IdRole,
                                NameRole,
                                ImageRole,
                                PriceRole,
                                InfoRole,
                                StatusRole
                        };
                        dataChanged(index(0, 0),
                                    index(rowCount() - 1, 0),
                                    roles);
                    });
        }
    }

    endResetModel();
}

void MenuModel::setCartList(CartList *cList) {
    beginResetModel();

    if (cartList)
        cartList->disconnect(this);

    cartList = cList;

    if (cartList) {
        if (displayMode == ShowMenu) {
            connect(cartList,
                    &CartList::itemCountChanged,
                    this,
                    [=](int id) {
                        int ind = menuList->getIndex(id);
                        dataChanged(index(ind, 0), index(ind, 0),
                                    QVector<int>() << CountRole);
                    });
            connect(cartList,
                    &CartList::indexesChanged,
                    this,
                    [=]() {
                        dataChanged(index(0, 0), index(rowCount() - 1, 0),
                                    QVector<int>() << CountRole);
                    });

        } else {
            connect(cartList,
                    &CartList::itemCountChanged,
                    this,
                    [=](int id) {
                        int ind = cartList->getIndex(id);
                        if (ind == -1) return;
                        dataChanged(index(ind, 0), index(ind, 0),
                                    QVector<int>() << CountRole);
                    });
            connect(cartList,
                    &CartList::indexesChanged,
                    this,
                    &MenuModel::updatePersistentIndexList);
            CONNECT_SIGNALS(cartList, CartList);
        }
    }

    endResetModel();
}

void MenuModel::setDisplayMode(int mode) {
    displayMode = mode;
}

void MenuModel::updatePersistentIndexList() {
    QModelIndexList newList{};
    if (displayMode == ShowMenu) {
        newList.reserve(menuList->size());
        int ind = 0;
        while (ind < menuList->size()) {
            newList.push_back(index(ind++, 0));
        }
    }
    changePersistentIndexList(persistentIndexList(), newList);
}
}
