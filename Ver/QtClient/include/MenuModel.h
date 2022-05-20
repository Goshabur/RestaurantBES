#pragma once

#include <QAbstractListModel>
#include "fwd.h"

namespace restbes {

class MenuModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(restbes::MenuList *menuList READ getMenuList WRITE setMenuList NOTIFY menuListChanged);
    Q_PROPERTY(restbes::CartList *cartList READ getCartList WRITE setCartList NOTIFY cartListChanged);
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

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] MenuList *getMenuList() const;

    [[nodiscard]] CartList *getCartList() const;

    void setMenuList(MenuList *mList);

    void setCartList(CartList *cList);

    void updatePersistentIndexList(const QModelIndexList &newList);

signals:
    void menuListChanged();

    void cartListChanged();

    void menuChanged();

private:
    MenuList *menuList;
    CartList *cartList;
};

}
