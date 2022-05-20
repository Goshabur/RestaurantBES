#pragma once

#include <QAbstractListModel>

#include "fwd.h"

namespace restbes {

class MenuModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(restbes::MenuList *menuList READ getMenuList WRITE setMenuList NOTIFY menuListChanged);
    Q_PROPERTY(restbes::CartList *cartList READ getCartList WRITE setCartList NOTIFY cartListChanged);
    Q_PROPERTY(int displayMode READ getDisplayMode WRITE setDisplayMode);
public:
    explicit MenuModel(QObject *parent = nullptr);

    enum {
        IdRole = Qt::UserRole,
        NameRole,
        ImageRole,
        PriceRole,
        InfoRole,
        StatusRole,
        CountRole
    };

    enum MenuDisplayMode {
        ShowCart,
        ShowMenu
    };
    Q_ENUM(MenuDisplayMode);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] MenuList *getMenuList() const;

    [[nodiscard]] CartList *getCartList() const;

    [[nodiscard]] int getDisplayMode() const;

    void setMenuList(MenuList *mList);

    void setCartList(CartList *cList);

    void setDisplayMode(int mode);

    void updatePersistentIndexList(const QModelIndexList &newList);

    Q_INVOKABLE void setItemCount(int id, int value);

    Q_INVOKABLE void increaseItemCount(int id);

    Q_INVOKABLE void decreaseItemCount(int id);

signals:
    void menuListChanged();

    void cartListChanged();

    void menuChanged();

private:
    MenuList *menuList = nullptr;
    CartList *cartList = nullptr;
    int displayMode = ShowMenu;
};

}
