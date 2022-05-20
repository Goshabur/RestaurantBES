#pragma once

#include <QObject>

#include <folly/Synchronized.h>

#include <set>
#include <memory>

#include "fwd.h"
#include "MenuData.h"

namespace restbes {

struct CartItem {
    int item_id;
    int count = 1;
};

class CartList : public QObject {
Q_OBJECT
public:
    explicit CartList(QObject *parent = nullptr);

    [[nodiscard]] int size() const;

    [[nodiscard]] CartItem getItemAt(int index) const;

    [[nodiscard]] int getIndex(int id) const;

    [[nodiscard]] int getId(int index) const;

    void updateIndexes();

    void clearCart();

    bool setItemCount(int id, int value);

    [[nodiscard]] int getItemCount(int id) const;

signals:
    void beginChangeLayout();

    void endChangeLayout();

    void beginInsertItem(int index);

    void endInsertItem();

    void beginRemoveItem(int index);

    void endRemoveItem();

    void itemCountChanged(int id);

    void cartCleared();

public slots:

private:
    folly::Synchronized<std::vector<CartItem>> items;
    folly::Synchronized<std::unordered_map<int, int>> indexes;
};

}
