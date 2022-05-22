#pragma once

#include <QObject>

#include <nlohmann/json.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "fwd.h"
#include "CartList.h"

#include <memory>

namespace restbes {

class Client : public QObject {
Q_OBJECT
    Q_PROPERTY(bool regStatus READ getRegStatus NOTIFY regStatusChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
    Q_PROPERTY(int userId READ getUserId NOTIFY userIdChanged)
    Q_PROPERTY(int sessionId READ getSessionId NOTIFY sessionIdChanged)
public:
    explicit Client(std::string server, int _port, QObject *parent = nullptr);

    [[nodiscard]] bool getRegStatus() const;

    [[nodiscard]] QString getName() const;

    [[nodiscard]] QString getEmail() const;

    [[nodiscard]] int getUserId() const;

    [[nodiscard]] int getSessionId() const;

    void startPolling();

    Q_INVOKABLE bool registerUser(const QString &regEmail,
                                  const QString &regPassword,
                                  const QString &regName);

    Q_INVOKABLE bool signInUser(const QString &email,
                                const QString &password);

    Q_INVOKABLE restbes::MenuList *getMenu();

    Q_INVOKABLE restbes::CartList* getCart() const;

// TODO:
//    Q_INVOKABLE void setItemCount(int id, int value);
//    Q_INVOKABLE void increaseItemCount(int id);
//    Q_INVOKABLE void decreaseItemCount(int id, int value);
//    Q_INVOKABLE void clearCart();
//    Q_INVOKABLE void createOrder(QString address, QString comment);

signals:

    void regStatusChanged();

    void nameChanged();

    void emailChanged();

    void userIdChanged();

    void sessionIdChanged();

    void exampleEvent();

public slots:

private:
    bool regStatus = false;
    QString name;
    QString email;
    int userId = -1;
    int sessionId = -1;
    std::string address;
    int port;
    folly::Synchronized<httplib::Headers> headers;
    std::shared_ptr<CartList> cartList = std::make_shared<CartList>();

    // remove after replacing all MenuList* with shared_ptrs in Qt
    std::shared_ptr<MenuList> menuList = nullptr;

    std::shared_ptr<httplib::Client> postingClient;
    std::shared_ptr<httplib::Client> pollingClient;
    std::shared_ptr<std::thread> pollingThread;

    enum PollingEvent {
        ItemCountChanged,
        OrderStatusChanged,
        NewOrder,
        AddMenuItem,
        RemoveMenuItem,
        ChangeMenuItem,
        ReloadMenu,
        ExampleEvent
    };

    static inline std::unordered_map<std::string, PollingEvent> eventMap{
            {"item_count_changed", ItemCountChanged},
            {"order_status_changed", OrderStatusChanged},
            {"new_order", NewOrder},
            {"add_menu_item", AddMenuItem},
            {"remove_menu_item", RemoveMenuItem},
            {"change_menu_item", ChangeMenuItem},
            {"reload_menu", ReloadMenu},
            {"example_event", ExampleEvent}
    };

    void setRegStatus(bool newStatus);

    void setName(QString newName);

    void setEmail(QString newEmail);

    void setUserId(int newId);

    void setSessionId(int newId);

    bool parseUserFromJson(const nlohmann::json &json);

    bool parseUserFromJson(const std::string &input);

    void getMenuFromServer();

};

}
