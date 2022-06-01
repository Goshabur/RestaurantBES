#pragma once

#include <memory>

#include <QObject>
#include <QString>

#include "CartListFWD.h"

namespace restbes {

class Order : public QObject {
Q_OBJECT
    Q_PROPERTY(int orderId READ getOrderId WRITE setOrderId NOTIFY orderIdChanged)
    Q_PROPERTY(uint32_t timestamp READ getTimestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(uint32_t lastModified READ getLastModified WRITE setLastModified NOTIFY lastModifiedChanged)
    Q_PROPERTY(int totalCost READ getTotalCost WRITE setTotalCost NOTIFY totalCostChanged)
    Q_PROPERTY(int status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString address READ getAddress WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString comment READ getComment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(CartList* cart READ getCart WRITE setCart NOTIFY cartChanged)

public:
    // sed -En '/([\_A-Za-z]+) ([A-Za-z]+);/p' Order.h | sed -E 's/([\_A-Za-z]+) ([A-Za-z]+);/\1 get\u\2() const;\n/'
    void setOrderId(int newOrderId);

    void setTimestamp(uint32_t newTimestamp);

    void setLastModified(uint32_t newLastModified);

    void setTotalCost(int newTotalCost);

    void setStatus(int newStatus);

    void setAddress(QString newAddress);

    void setComment(QString newComment);

    void setCart(CartList* newCart);

    [[nodiscard]] int getOrderId() const;

    [[nodiscard]] uint32_t getTimestamp() const;

    [[nodiscard]] uint32_t getLastModified() const;

    [[nodiscard]] int getTotalCost() const;

    [[nodiscard]] int getStatus() const;

    [[nodiscard]] QString getAddress() const;

    [[nodiscard]] QString getComment() const;

    [[nodiscard]] CartList* getCart() const;



signals:
    void orderIdChanged();

    void timestampChanged();

    void lastModifiedChanged();

    void totalCostChanged();

    void statusChanged();

    void addressChanged();

    void commentChanged();

    void cartChanged();

private slots:

private:
    int orderId;
    uint32_t timestamp;
    uint32_t lastModified;
    int totalCost;
    int status;
    QString address;
    QString comment;
    CartList* cart;
};

}