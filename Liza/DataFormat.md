# Формат данных в POST-запросах

- [postAuthorizationMethodHandler](#Регистрация/авторизация)
- [postCartMethodHandler](#Изменение корзины)
- [postOrderMethodHandler](#Обработка заказа)

## Регистрация/авторизация

`sign_in`/`sign_up`

Для `sign_in` имя не передаётся, поэтому достаточно в `name` передать пустую строку

```json
{
  "query": "sign_in",
  "body": {
    "name": "Alice",
    "email": "example@mail.ru",
    "password": "password"
  }
}
```

## Изменение корзины

`set_item_count`/`clear_cart`/`show_cart`

_"body"_ нужен только для `set_item_count`

```json
{
  "query": "set_item_count",
  "body": {
    "dish_id": 1,
    "count": 1
  }
}
```

## Обработка заказа

`create_order`/`show_order_status`

Если пользователь не залогинен, его нужно будет залогинить, а значит мне нужны данные для регистрации (`create_order` можно разбить на два запроса: `sign_up` и `create_order` уже для зарегистрированного пользователя, будет проще и логичнее, понадобится только значение поля _"query"_)

_"order_id"_ только для `show_order_status`

```json
{
  "query": "create_order",
  "logged_in": 0,
  "body": {
    "order_id": 2,
    "name": "Alice",
    "email": "example@mail.ru",
    "password": "password",
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