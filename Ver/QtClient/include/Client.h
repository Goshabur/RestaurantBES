#pragma once

#include <QObject>

#include <nlohmann/json.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"

#include "MenuList.h"
#include "CartList.h"
#include "OrderList.h"
#include "Order.h"

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
    enum ServerNotification : bool {
        NotifyServer = true,
        DontNotifyServer = false
    };
    Q_ENUM(ServerNotification);

    explicit Client(QObject *parent = nullptr);

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

    Q_INVOKABLE restbes::MenuList *getMenu() const;

    Q_INVOKABLE restbes::CartList *getCart() const;

    Q_INVOKABLE restbes::OrderList *getOrderList() const;

    Q_INVOKABLE void clearCart(bool notifyServer);

    Q_INVOKABLE void setItemCount(int id, int value);

    Q_INVOKABLE void increaseItemCount(int id);

    Q_INVOKABLE void decreaseItemCount(int id);

    Q_INVOKABLE void createOrder(const QString &addr, const QString &commnt);

private slots:
    void getOrderFromServer(int orderId);

signals:

    void regStatusChanged();

    void nameChanged();

    void emailChanged();

    void userIdChanged();

    void sessionIdChanged();

    void newOrder(restbes::Order* order);

    void orderStatusChanged(restbes::Order* order);

    void getOrder(int orderId);

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
    std::shared_ptr<MenuList> menuList = std::make_shared<MenuList>();
    std::shared_ptr<OrderList> orderList = std::make_shared<OrderList>();

    std::shared_ptr<httplib::Client> postingClient;
    std::shared_ptr<httplib::Client> pollingClient;
    std::shared_ptr<std::thread> pollingThread;

    enum PollingEvent {
        CartChanged,
        OrderChanged,
        MenuChanged,
        NewSignIn
    };

    static inline std::unordered_map<std::string, PollingEvent> eventMap{
            {"cart_changed",  CartChanged},
            {"order_changed", OrderChanged},
            {"menu_changed",  MenuChanged},
            {"new_sign_in",   NewSignIn}
    };

    void setRegStatus(bool newStatus);

    void setName(QString newName);

    void setEmail(QString newEmail);

    void setUserId(int newId);

    void setSessionId(int newId);

    bool parseUserFromJson(const nlohmann::json &json);

    bool parseUserFromJson(const std::string &input);

    void getMenuFromServer();

    void getCartFromServer();

};

}
