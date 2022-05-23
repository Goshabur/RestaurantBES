# Формат ответов

- [get_menu](#Запрос-меню)
- [sign_in/sign_up/get_user](#Регистрация/авторизация/запрос-пользователя)
- [get_cart](#Запрос-корзины)
- [get_order](#Запрос-заказа)

## Запрос меню

`timestamp` - дата последнего обновления меню
```json
{
  "query": "get_menu",
  "status_code": 0,
  "body": {
    "item": "menu",
    "timestamp": "2022/01/25 22:35:06",
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
        "timestamp": "2022/01/25 22:35:06"
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
    "timestamp": "2022/01/25 22:35:06",
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