#pragma once

#include <QObject>

#include <memory>

namespace restbes {

struct OrderItem {
    const int order_id = -1;
    int status = -1;
    const unsigned int date = 0;

    friend bool operator==(OrderItem a, OrderItem b) {
        return a.order_id == b.order_id && a.status == b.status &&
               a.date == b.date;
    }

    friend bool operator!=(OrderItem a, OrderItem b) {
        return !(a == b);
    }
};

using OrderData = std::vector<OrderItem>;

class OrderList : public QObject {
Q_OBJECT
public:
    explicit OrderList(QObject *parent = nullptr);

    [[nodiscard]] int size() const;

    [[nodiscard]] OrderItem getItemAt(int index) const;

    [[nodiscard]] OrderItem getItem(int id) const;

    [[nodiscard]] int getIndex(int id) const;

    [[nodiscard]] int getId(int index) const;

    void setItemStatus(int id, int value, unsigned int date = 0);

    void setOrderData(std::shared_ptr<OrderData> newData);

    [[nodiscard]] int getItemStatus(int id) const;

    [[nodiscard]] OrderData::const_iterator begin() const;

    [[nodiscard]] OrderData::const_iterator end() const;

    unsigned int getTimestamp() const;

    void setTimestamp(unsigned int newTimestamp);

signals:

    void beginChangeLayout();

    void endChangeLayout();

    void beginInsertItem(int index);

    void endInsertItem();

    void beginRemoveItem(int index);

    void endRemoveItem();

    void itemChanged(int id);

private:
    std::shared_ptr<OrderData> orderData = std::make_shared<OrderData>();
    std::unordered_map<int, int> indexes;
    unsigned int timestamp = 0;
};

}