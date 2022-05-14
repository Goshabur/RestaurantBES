#pragma once

#include <QObject>
//#include "httplib.h"

namespace restbes {

class Client : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool regStatus READ getRegStatus WRITE setRegStatus NOTIFY regStatusChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
public:
    explicit Client(QObject *parent = nullptr);
    bool getRegStatus();
    QString getName();
    Q_INVOKABLE void increment();

signals:
    void incremented();
    void regStatusChanged();
    void nameChanged();

public slots:

    void setRegStatus(bool newStatus);
    void setName(QString newName);

private:
    bool regStatus = false;
    QString name;
};

}
