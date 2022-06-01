#pragma once

#include <QObject>

#include <memory>

namespace restbes {

struct OrderItem {
    const int order_id = -1;
    int status = -1;
    const uint32_t date = 0;

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

    bool setItemStatus(int id, int value);

    void setOrderData(std::shared_ptr<OrderData> newData);

    [[nodiscard]] int getItemStatus(int id) const;

    [[nodiscard]] OrderData::const_iterator begin() const;

    [[nodiscard]] OrderData::const_iterator end() const;

    uint32_t getTimestamp() const;

    void setTimestamp(uint32_t newTimestamp);

signals:

    void beginChangeLayout();

    void endChangeLayout();

    void beginInsertItem(int index);

    void endInsertItem();

    void beginRemoveItem(int index);

    void endRemoveItem();

    void itemChanged(int id);

private:
    std::shared_ptr<OrderData> orderData;
    std::unordered_map<int, int> indexes;
    uint32_t timestamp;
};

}