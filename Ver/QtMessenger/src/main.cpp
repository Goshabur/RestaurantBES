#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QStringLiteral>
#include <QQmlContext>

#include <thread>

#include "MenuList.h"
#include "MenuModel.h"
//#include "client.h"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
//    qmlRegisterType<restbes::Client>("Ver", 1, 0, "Client");
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
    std::map<int, restbes::MenuItem> menuMap =
            {{1, item[0]},
             {2, item[1]}};
    restbes::MenuList menu;
    menu.resetList(menuMap);


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("menu"), &menu);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    engine.load(url);

//    std::thread t([&menu]() {
//          restbes::MenuItem item[2] =
//                  {{1,
//                   "Салат \"Цезарь\"",
//                   "https://ogorodland.ru/wp-content/uploads/2021/11/salat-cezar-klassicheskii-s-kuricei_1611309080_15_max.jpg",
//                   100,
//                   "Description",
//                   1},
//                   {2,
//                   "Гренки с чесноком",
//                   "https://kartinkin.net/uploads/posts/2021-03/1616105233_38-p-grenki-krasivaya-yeda-41.jpg",
//                   200,
//                   "Description",
//                   1}};
//          std::map<int, restbes::MenuItem> menuArray[2] =
//                  {{{1, item[0]}},
//                   {{2, item[1]}}};
//          int i = 0;
//          while (++i) {
//              std::this_thread::sleep_for(1s);
//              restbes::MenuItem newItem = item[i & 1];
//              newItem.id = i;
//              menu.insertItem(newItem);
//          }
//      }
//    );
//    t.detach();

    return app.exec();

}

