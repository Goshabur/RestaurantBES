#include "Order.h"

#include <utility>
#include <CartList.h>

namespace restbes {

int Order::getOrderId() const { return orderId; }

unsigned int Order::getTimestamp() const { return timestamp; }

unsigned int Order::getLastModified() const { return lastModified; }

int Order::getTotalCost() const { return totalCost; }

int Order::getStatus() const { return status; }

QString Order::getAddress() const { return address; }

QString Order::getComment() const { return comment; }

CartList* Order::getCart() const { return cart; }


void Order::setOrderId(int newOrderId) {
    if (orderId == newOrderId) return;
    orderId = newOrderId;
    emit orderIdChanged();
}

void Order::setTimestamp(unsigned int newTimestamp) {
    if (timestamp == newTimestamp) return;
    timestamp = newTimestamp;
    emit timestampChanged();
}

void Order::setLastModified(unsigned int newLastModified) {
    if (lastModified == newLastModified) return;
    lastModified = newLastModified;
    emit lastModifiedChanged();
}

void Order::setTotalCost(int newTotalCost) {
    if (totalCost == newTotalCost) return;
    totalCost = newTotalCost;
    emit totalCostChanged();
}

void Order::setStatus(int newStatus) {
    if (status == newStatus) return;
    status = newStatus;
    emit statusChanged();
}

void Order::setAddress(QString newAddress) {
    if (address == newAddress) return;
    address = std::move(newAddress);
    emit addressChanged();
}

void Order::setComment(QString newComment) {
    if (comment == newComment) return;
    comment = std::move(newComment);
    emit commentChanged();
}

void Order::setCart(CartList* newCart) {
    if (cart == newCart) return;
    cart = newCart;
    emit cartChanged();
}

}
