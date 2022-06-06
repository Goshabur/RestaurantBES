# RestaurantBES
HSE first year C++ project

# Содержание
- [Клиенты и их возможности](#Функции-клиентов)
- [Схема БД](#Схема-БД)
- [Архитектура клиента](#Архитектура-клиента)
- [Архитектура сервера](#Архитектура-сервера)
- [Схема сетей](#Схема-сетей)
- [Обработка запросов клиента](#Обработка-запросов-клиента)
- [Сценарии](#Обрабатываем-3-сценария)
- [Запросы клиента](#Запросы-клиента)
- [Формат данных в POST-запросах](#Формат-данных-в-POST-запросах)
- [Формат ответов](#Формат-ответов)
- [Формат уведомлений](#Формат-уведомлений)
- [Сборка](#Сборка)

# Функции клиентов
Две роли — заказчик (через приложение) и администратор (через телеграм-бота).

**Возможности заказчика:**
- Регистрация/авторизация
- Просмотр меню
- Изменение корзины
    - Добавление/удаление товара
    - Сброс корзины
- Оформление заказа
- Просмотр истории заказов
- Просмотр информации о конкретном заказе 

**Возможности администратора:**
- Авторизация
- Изменение меню
    - Изменение стоимости товара
    - Изменение статуса товара
    - Добавление нового товара
- Изменение статуса заказа

# Архитектура клиента
![Архитектура-клиента](https://github.com/Goshabur/RestaurantBES/blob/main/ITServices.png)

# Архитектура сервера
![Архитектура-сервера](https://github.com/Goshabur/RestaurantBES/blob/main/ServerUML.png)

# Схема БД
![Схема-БД](https://github.com/Goshabur/RestaurantBES/blob/main/DataBaseUML.png)

# Схема сетей
![Схема-сетей](https://github.com/Goshabur/RestaurantBES/blob/main/NetworkDiagram.png)

# Обработка запросов клиента

Запросы бывают двух типов:

1. **GET-запросы** с целью получения актуальных данных
2. **POST-запросы** для внесения изменений на сервере

Посылки сервера так же бывают двух типов:

1. **Ответ** — посылается только сессии-источнику первоначального запроса
2. **Уведомление** — посылается во все открытые принимающие сессии пользователя

На сервере у корзины и у каждого заказа должен быть timestamp — время последнего изменения с точки зрения сервера

# Обрабатываем 3 сценария:

1. **Запрос данных**
    - Клиент посылает GET-запрос.
    - Сервер присылает ответ.
2. **Изменение данных**
    - Клиент посылает POST-запрос.
    - Сервер обрабатывает запрос и присылает ответ содержащий итог обработки (успех/неуспех) и timestamp, который был присвоен этому изменению.
    - В случае успеха сервер также посылает пользователю уведомление с информацией о том, какие данные изменились, и timestamp-ом изменения.
    - Все клиенты, получившие уведомление, сравнивают timestamp изменения с timestamp-ом имеющихся данных и в случае несоответствия отправляют соответствующие GET-запросы.
    - Сервер рассылает ответы на полученные GET-запросы.
3. **Регистрация/авторизация**
    - Клиент посылает POST-запрос с данными для регистрации/авторизации.
    - Сервер обрабатывает запрос, присылает ответ с итогом обработки запроса.
    - В случае успеха формат ответа должен совпадать с форматом ответа на запрос `sign_in`.
    - ОПЦИОНАЛЬНО: сервер посылает пользователю уведомление о новом входе/попытке входа в аккаунт.

# Запросы клиента

1. **GET-запросы**
    - `get_menu`
        - resource: /menu
    - `get_order`
        - resource: /order
    - `get_cart`
        - resource: /cart
2. **POST-запросы**
    - `set_item_count`
        - resource: /cart
    - `set_cart`
        - resource: /cart
    - `create_order`
        - resource: /order
    - `sign_in`
        - resource: /user
    - `sign_up`
        - resource: /user

# Формат данных в POST-запросах

- [sign_in/sign_up](#Регистрация/авторизация)
- [set_item_count/set_cart](#Изменение-корзины)
- [create_order](#Обработка-заказа)

## Регистрация/авторизация

`sign_in`/`sign_up`

Для `sign_in` имя не передаётся, поэтому достаточно в `name` передать пустую строку

Также в случае `sign_in` передаётся поле `update_cart` = 1, если корзина, которую пользователь набрал, будучи не залогиненным, изменялась позже, чем корзина, сохранённая на аккаунте

Для `sign_up` поле `update_cart` указывает на то, что корзина не пуста, и соответственно её нужно обновить (по умолчанию корзина пуста)

```json
{
  "query": "sign_in",
  "body": {
    "name": "Alice",
    "email": "example@mail.ru",
    "password": "password",
    "update_cart": 1,
    "cart": [
      {
        "dish_id": 1,
        "count": 1
      },
      ...
    ]
  }
}
```

## Изменение корзины

`set_item_count`/`set_cart`

**"cart"** нужен только для `set_cart` (ненужные поля для команд оставить пустыми)

```json
{
  "query": "set_item_count",
  "body": {
    "dish_id": 1,
    "count": 1,
    "cart": [
      {
        "dish_id": 1,
        "count": 1
      },
      ...
    ]
  }
}
```

## Обработка заказа

Остальная информация (имя, корзина и т.п.) не нужна, так как её можно получить по айдишнику пользователя

```json
{
  "query": "create_order",
  "body": {
    "address": "Somewhere in Germany",
    "comment": "Something good (or not)"
  }
}
```

# Формат ответов

- [get_menu](#Запрос-меню)
- [sign_in/sign_up](#Регистрация/авторизация)
- [get_cart](#Запрос-корзины)
- [get_order](#Запрос-заказа)
- [set_item_count/set_cart/create_order](#POST-запрос)

## Запрос меню

`timestamp` — дата последнего обновления меню в **seconds since epoch**
```json
{
  "query": "get_menu",
  "status_code": 0,
  "body": {
    "item": "menu",
    "timestamp": "34680923",
    "contents": [
      {
        "item": "dish",
        "dish_id": 0,
        "name": "Lasagna",
        "image": "https://i.imgur.com/SPHmdBR.jpg",
        "price": 100,
        "info": "Some kind of a description",
        "status": 1
      },
      ...
    ]
  }
}
```

## Регистрация/авторизация

Аналогичный ответ для `sign_up`

`timestamp` — дата заказа
```json
{
  "query": "sign_in",
  "status_code": 0,
  "body": {
    "item": "user",
    "user_id": 0,
    "name": "Alice",
    "email": "alice@gmail.com",
    "orders": [
      {
        "order_id": 0,
        "status": 2,
        "timestamp": "34680923",
        "last_modified": "34680923"
      },
      ...
    ]
  }
}
```

## Запрос корзины

```json
{
  "query": "get_cart",
  "status_code": 0,
  "body": {
    "item": "cart",
    "timestamp": "34680923",
    "contents": [
      {
        "dish_id": 0,
        "count": 1
      },
      ...
    ]
  }
}
```

## Запрос заказа

```json
{
  "query": "get_order",
  "status_code": 0,
  "body": {
    "item": "order",
    "order_id": 0,
    "timestamp": "34680923",
    "last_modified": "34680923",
    "cost": 1430,
    "status": 0,
    "address": "Somewhere in Israel",
    "comment": "Something you don't know what",
    "cart": {
      "item": "cart",
      "contents": [
        {
          "dish_id": 0,
          "count": 1
        },
        ...
      ]
    }
  }
}
```

## Ошибка

```json
{
  "query": "...",
  "status_code": 1,
  "body": {
    "item": "error",
    "error_code": 1,
    "body": {
      ...
    }
  }
}
```

## POST-запрос

Формат ответа для `set_item_count`, `set_cart` и `create_order`

```json
{
  "query": "cart_changed",
  "status_code": 0,
  "timestamp": "34680923"
}
```

# Формат уведомлений

- [cart_changed](#Изменилась-корзина)
- [order_changed](#Изменился-заказ)
- [menu_changed](#Изменилось-меню)
- ОПЦИОНАЛЬНО: [new_sign_in](#Новая-попытка-входа)

## Изменилась корзина

```json
{
  "event": "cart_changed",
  "timestamp": "34680923"
}
```

## Изменился заказ

Сюда входят в частности изменение статуса и создание нового заказа

```json
{
  "event": "order_changed",
  "timestamp": "34680923",
  "body": {
    "order_id": 1
  }
}
```

## Изменилось меню

```json
{
  "event": "menu_changed",
  "timestamp": "34680923"
}
```

## Новая попытка входа

```json
{
  "event": "new_sign_in",
  "timestamp": "34680923",
  "body": {
    "query": "sign_in",
    "status": 0
  }
}
```

# Сборка

- [Библиотеки](#Необходимые-библиотеки)
- [Флаги](#Флаги)
- [Запуск сервера](#Запуск-сервера)
- [Запуск клиента](#Запуск-клиента)

### Необходимые библиотеки:
- [restbed](https://github.com/Corvusoft/restbed) — асинхронная работа с HTTPs запросами на сервере. См. [build](https://github.com/Corvusoft/restbed#build)
- [nlohmann/json](https://github.com/nlohmann/json#embedded-fetchcontent) — см. [CMakeLists.txt](https://github.com/Goshabur/RestaurantBES/blob/main/Liza/CMakeLists.txt)
- [PostgreSQL](https://www.digitalocean.com/community/tutorials/how-to-install-postgresql-on-ubuntu-20-04-quickstart) — реляционная база данных. См. шаги 1 и 2. Затем необходимо установить пароль командой `\password postgres`. Далее см. шаг 4. После этого необходимо настроить порты и разрешить подключение **не** только с локальной машины (на ваше усмотрение):
  - `$ sudo gedit /etc/postgresql/12/main/postgresql.conf`
  - Меняем строчку `listen_addresses = '*'`
  - Настроить порты можно командой `$ ss -nlt` (в нашем случае это порт 5432 и IP-адрес удаленного сервера). Убедитесь, что ваш IP-адрес есть в списке.
  - Перезапускаем, чтобы сохранить изменения:
   
    `$ sudo systemctl restart postgresql`
  - База данных готова к использованию. Подключение к БД на удаленном сервере выполняется командой `$ psql -U<имя_пользователя> -h<IP-адрес> -d<имя_БД>`
  - См. [список команд](https://github.com/Goshabur/RestaurantBES/blob/main/Liza/sql_query.txt), чтобы наполнить базу данных
  - Библиотека [libpqxx](https://github.com/jtv/libpqxx#building-libpqxx) уже есть в [CMakeLists.txt](https://github.com/Goshabur/RestaurantBES/blob/main/Liza/CMakeLists.txt), дополнительных действий не требуется
- Qt — интерфейс клиента:
  - `$ sudo apt-get install qt5-default`
  - `$ sudo apt-get install qml-module-qtquick-dialogs qml-module-qtquick-controls2 qml-module-qtquick-layouts qml-module-qtquick-window2` 
- [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp) — см. [установку](https://github.com/reo7sp/tgbot-cpp#library-installation). Больше ничего не требуется, [CMakeLists.txt](https://github.com/Goshabur/RestaurantBES/blob/main/Liza/CMakeLists.txt) уже настроен
- [gflags](https://github.com/gflags/gflags) — настройка флагов для запуска из консоли:
  - `$ sudo apt-get install libgflags-dev`
- [folly](https://github.com/facebook/folly) — потокобезопасность ([folly::Synchronized](https://github.com/facebook/folly/blob/main/folly/docs/Synchronized.md)) и работа с JSON ([folly::dynamic](https://github.com/facebook/folly/blob/main/folly/docs/Dynamic.md)). См. [build](https://github.com/facebook/folly#build). Необходимо настроить пути до папок folly, как это сделано в [CMakeLists.txt](https://github.com/Goshabur/RestaurantBES/blob/main/Liza/CMakeLists.txt). Будьте внимательны, иначе потратите кучу нервов :) А еще надо иметь в виду, что установка folly съедает немалое количество места на диске, поэтому более разумным будет установка только необходимых библиотек (см. используемые библиотеки в [CMakeLists.txt](https://github.com/Goshabur/RestaurantBES/blob/main/Liza/CMakeLists.txt))

### Флаги

```shell
--port N # Номер порта (0 < N < 32768), обязательный

--SSLkeys /GLOBAL/PATH # Путь до папки с ключами и сертификатом для соединения по протоколу https, обязательный

--workers # Максимальное количество потоков (0 < n < 100), по умолчанию 10
```

### Запуск сервера
```
$ cmake --build . --target RestaurantBES

$ ./RestaurantBES --port <N> --SSLkeys </GLOBAL/PATH>
```

### Запуск клиента
```
$ cmake --build . --target QtClient

$ ./QtClient
```