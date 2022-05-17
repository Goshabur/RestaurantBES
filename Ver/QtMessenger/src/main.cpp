#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QStringLiteral>
#include <QQmlContext>
#include "MenuList.h"
#include "MenuModel.h"
//#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    qmlRegisterType<restbes::Client>("Ver", 1, 0, "Client");
    qmlRegisterType<restbes::MenuModel>("Ver", 1, 0, "MenuModel");
    qmlRegisterType<restbes::MenuList>("Ver", 1, 0, "MenuList");

    restbes::MenuList menu;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("menu"), &menu);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    engine.load(url);

    return app.exec();
}

