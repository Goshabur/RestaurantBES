#pragma once

#include <QAbstractListModel>
#include "fwd.h"

namespace restbes {

class CartModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(restbes::CartList *list READ list WRITE setList NOTIFY listChanged)

public:
    explicit CartModel(QObject *parent = nullptr);

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

    CartList *list() const;

    void setList(CartList *list);

    void updatePersistentIndexList(const QModelIndexList &newList);

signals:
    void listChanged();

//    void cartChanged();

private:
    CartList *cartList;
};

}
