#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QStringLiteral>
#include <QQmlContext>

#include <thread>

#include "MenuList.h"
#include "MenuModel.h"
#include "MenuData.h"
#include "CartList.h"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qmlRegisterType<restbes::CartList>("Ver", 1, 0, "CartList");
    qmlRegisterType<restbes::MenuModel>("Ver", 1, 0, "MenuModel");
    qmlRegisterType<restbes::MenuList>("Ver", 1, 0, "MenuList");

    restbes::MenuItem item[2] =
            {{1,
             "Салат \"Цезарь\"",
             "https://ogorodland.ru/wp-content/uploads/2021/11/salat-cezar-klassicheskii-s-kuricei_1611309080_15_max.jpg",
             100,
             "This is a very long description, and extremely long description, so long that no one will ever read it",
             1},
             {2,
             "Гренки с чесноком",
             "https://kartinkin.net/uploads/posts/2021-03/1616105233_38-p-grenki-krasivaya-yeda-41.jpg",
             200,
             "This is a very long description, and extremely long description, so long that no one will ever read it",
             1}};
    std::unordered_map<int, restbes::MenuItem> menuMap =
            {{1, item[0]},
             {2, item[1]}};
    auto menuData = std::make_shared<restbes::MenuData>(std::move(menuMap));
    auto menuList = std::make_shared<restbes::MenuList>();
    auto cartList = std::make_shared<restbes::CartList>();
    menuList->setMenu(menuData);
    cartList->setItemCount(1, 1);
    cartList->setItemCount(2, 0);


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("menu"), menuList.get());
    engine.rootContext()->setContextProperty(QStringLiteral("theCart"), cartList.get());

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    engine.load(url);

    std::thread t([&menuList, &item]() {
          int i = 2;
          while (++i) {
              std::this_thread::sleep_for(3s);
              restbes::MenuItem newItem = item[!(i & 1)];
              newItem.id = i;
              menuList->insertItem(newItem);
          }
      }
    );
    t.detach();

    return app.exec();

}

