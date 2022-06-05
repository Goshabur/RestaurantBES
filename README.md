# RestaurantBES
HSE first year project

# Содержание
- [Клиент-Сервер](#Клиент-Сервер)
- [Схема БД](#Схема-БД)
- [Архитектура](#Архитектура)
- [Схема сетей](#Схема-сетей)
- [Обработка запросов клиента](#Обработка-запросов-клиента)
- [Сценарии](#Обрабатываем-3-сценария)
- [Запросы клиента](#Запросы-клиента)
- [Формат ответов](#Формат-ответов)
- [Формат уведомлений](#Формат-уведомлений)

# Клиент-сервер
![Клиент-Сервер](https://github.com/Goshabur/RestaurantBES/blob/main/Client-ServerModel.png)

# Схема БД
![Схема-БД](https://github.com/Goshabur/RestaurantBES/blob/main/DataBaseUML.png)

# Архитектура
![Архитектура](https://github.com/Goshabur/RestaurantBES/blob/main/ServerUML.png)

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
