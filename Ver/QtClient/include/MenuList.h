#pragma once

#include <QObject>
#include <QModelIndexList>

#include "MenuItem.h"

#include <set>
#include <memory>

namespace restbes {

using MenuData = std::unordered_map<int, MenuItem>;

class MenuList : public QObject {
Q_OBJECT
public:
    explicit MenuList(QObject *parent = nullptr);

    [[nodiscard]] std::shared_ptr<MenuData> getMenuData() const;

    [[nodiscard]] int size() const;

    [[nodiscard]] int getId(int index) const;

    [[nodiscard]] int getIndex(int index) const;

    [[nodiscard]] std::set<int>::const_iterator begin() const;

    [[nodiscard]] std::set<int>::const_iterator end() const;

    bool setItemAt(int index, const MenuItem &item);

    [[nodiscard]] MenuItem getItemAt(int index) const;

    [[nodiscard]] MenuItem getItem(int id) const;

    void setMenu(std::shared_ptr<MenuData> newData);

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
    uint32_t timestamp = 0;
};

}
