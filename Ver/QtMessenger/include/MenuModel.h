#pragma once

#include <QAbstractListModel>
#include "fwd.h"

namespace restbes {

class MenuModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(restbes::MenuList *list READ list WRITE setList)

public:
    explicit MenuModel(QObject *parent = nullptr);

    enum {
        IdRole = Qt::UserRole,
        NameRole,
        ImageRole,
        PriceRole,
        InfoRole,
        StatusRole
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

    MenuList *list() const;

    void setList(MenuList *list);

private:
    MenuList *mList;
};

}
