#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QStringLiteral>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<restbes::Client>("Ver", 1, 0, "Client");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl){
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

