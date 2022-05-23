#pragma once

#include <QObject>

#include <memory>

namespace restbes {

struct OrderItem {
    int order_id = -1;
    int status = -1;
};

using OrderData = std::vector<OrderItem>;

class OrderList : public QObject {
Q_OBJECT
public:
    explicit OrderList(QObject* parent = nullptr);

    [[nodiscard]] int size() const;

    [[nodiscard]] OrderItem getItemAt(int index) const;

    [[nodiscard]] OrderItem getItem(int id) const;

    [[nodiscard]] int getIndex(int id) const;

    [[nodiscard]] int getId(int index) const;

    bool setItemStatus(int id, int value);

    void setOrderData(std::shared_ptr<OrderData> newData);

    [[nodiscard]] int getItemStatus(int id) const;

    [[nodiscard]] OrderData::const_iterator begin() const;

    [[nodiscard]] OrderData::const_iterator end() const;

signals:

    void beginChangeLayout();

    void endChangeLayout();

    void beginInsertItem(int index);

    void endInsertItem();

    void beginRemoveItem(int index);

    void endRemoveItem();

    void itemStatusChanged(int id);

private:
    std::shared_ptr<OrderData> orderData;
    std::unordered_map<int, int> indexes;
};

}