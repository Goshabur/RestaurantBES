#pragma once

#include <QObject>
#include <map>

namespace restbes {

struct MenuItem {
    int id;
    QString name;
    QString image;
    int price;
    QString info;
    int status;

    friend bool operator!=(const MenuItem &a, const MenuItem &b) {
        return a.id != b.id ||
               a.price != b.price ||
               a.status != b.status ||
               a.info != b.info ||
               a.image != b.image ||
               a.name != b.name;
    }

    friend bool operator==(const MenuItem &a, const MenuItem &b) {
        return !(a != b);
    }
};

class MenuList : public QObject {
Q_OBJECT
public:
    explicit MenuList(QObject *parent = nullptr);

    [[nodiscard]] std::map<int, MenuItem> items() const;

    [[nodiscard]] int size() const;

    bool setItemAt(int index, const MenuItem &item);

    std::map<int, MenuItem>::iterator findItemAt(int index);

    MenuItem getItemAt(int index);

signals:

    void beginChangeLayout();

    void endChangeLayout();

    void beginInsertItem(int index);

    void endInsertItem();

    void beginRemoveItem(int index);

    void endRemoveItem();

public slots:

    void insertItem(MenuItem item);

    void removeUnavailableItems();

private:
    std::map<int, MenuItem> mItems;
};

}
