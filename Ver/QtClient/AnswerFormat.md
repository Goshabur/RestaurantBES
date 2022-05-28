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

