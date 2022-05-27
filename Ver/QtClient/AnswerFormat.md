# Формат ответов

- [get_menu](#Запрос-меню)
- [get_user/sign_in/sign_up](#Регистрация/авторизация/запрос-пользователя)
- [get_cart](#Запрос-корзины)
- [get_order](#Запрос-заказа)
- [item_count_changed/clear_cart/create_order](#POST-запрос)

## Запрос меню

`timestamp` - дата последнего обновления меню в seconds since epoch
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
        "id": 0,
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

## Регистрация/авторизация/запрос пользователя

Аналогичный ответ для `sign_in` и `sign_up`

`timestamp` - дата заказа
```json
{
  "query": "get_user",
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
        "timestamp": "34680923"
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
        "id": 0,
        "quantity": 1
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
    "status": 0,
    "address": "Somewhere in Israel",
    "cart": {
      "item": "cart",
      ...
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

Аналогичный ответ для `clear_cart` и `create_order`

```json
{
  "query": "item_count_changed",
  "status_code": 0,
  "timestamp": "34680923"
}
```

