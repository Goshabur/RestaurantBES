#include "client.h"
#include <QDebug>

namespace restbes {

Client::Client(QObject *parent) : QObject(parent) {
    qDebug() << "Client created";
}

bool Client::getRegStatus() {
    return regStatus;
}

void Client::setRegStatus(bool newStatus) {
    if (newStatus != regStatus) {
        regStatus = newStatus;
        qDebug() << "regStatus = " << regStatus;
        emit regStatusChanged();
    }
}

QString Client::getName() {
    return name;
}

void Client::setName(QString newName) {
    if (newName != name) {
        name = std::move(newName);
        setRegStatus(!name.isEmpty());
        qDebug() << "name = " << name;
        emit nameChanged();
    }
}

void Client::increment() {
    emit incremented();
}

}