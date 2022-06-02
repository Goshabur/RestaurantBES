#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QStringLiteral>
#include <QQmlContext>

#include <thread>

#include <gflags/gflags.h>

#include "CartList.h"
#include "MenuList.h"
#include "MenuModel.h"
#include "OrderList.h"
#include "OrderModel.h"
#include "Client.h"

using namespace std::chrono_literals;

static bool ValidatePort(const char *flagname, gflags::int32 value) {
    if (value > 0 && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

DEFINE_int32(port, 1234, "What port to listen on");
DEFINE_string(server, "localhost", "Domain to send requests to");

DEFINE_validator(port, &ValidatePort);

int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    QApplication app(argc, argv);
    qmlRegisterType<restbes::CartList>("Ver", 1, 0, "CartList");
    qmlRegisterType<restbes::MenuModel>("Ver", 1, 0, "MenuModel");
    qmlRegisterType<restbes::MenuList>("Ver", 1, 0, "MenuList");
    qmlRegisterType<restbes::OrderModel>("Ver", 1, 0, "OrderModel");
    qmlRegisterType<restbes::OrderList>("Ver", 1, 0, "OrderList");
    qmlRegisterType<restbes::Client>("Ver", 1, 0, "Client");
    qmlRegisterType<restbes::Order>("Ver", 1, 0, "Order");

    auto client = std::make_shared<restbes::Client>(fLS::FLAGS_server,
                                                    fLI::FLAGS_port);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("theClient"),
                                             client.get());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);
    return app.exec();

}

