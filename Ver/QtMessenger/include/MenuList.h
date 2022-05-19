#pragma once

#include <QObject>
#include <QModelIndex>
#include <set>
#include <memory>
#include "fwd.h"
#include "MenuData.h"

namespace restbes {

class MenuList : public QObject {
Q_OBJECT
public:
    explicit MenuList(QObject *parent = nullptr);

    [[nodiscard]] std::shared_ptr<MenuData> getMenuData() const;

    [[nodiscard]] std::shared_ptr<CartList> getCartList() const;

    [[nodiscard]] int size() const;

    [[nodiscard]] int getItemCount(int id) const;

    [[nodiscard]] int getId(int index) const;

    [[nodiscard]] int getIndex(int index) const;

    bool setItemAt(int index, const MenuItem &item);

    MenuItem getItemAt(int index);

    void setMenu(std::shared_ptr<MenuData> newData);

    void setCart(std::shared_ptr<CartList> newData);

    bool setItemCount(int id, int value);

signals:
    void beginChangeLayout();

    void endChangeLayout();

    void beginInsertItem(int index);

    void endInsertItem();

    void beginRemoveItem(int index);

    void endRemoveItem();

    void itemChanged(int id);

public slots:

    void insertItem(restbes::MenuItem item);

    void removeItem(int id);

    void removeUnavailableItems();

private:
    std::set<int> items;
    std::shared_ptr<MenuData> menuData;
    std::shared_ptr<CartList> cartList;
};

}
