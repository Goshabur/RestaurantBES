# Формат данных в POST-запросах

- [sign_in/sign_up](#Регистрация/авторизация)
- [set_item_count/set_cart](#Изменение корзины)
- [create_order](#Обработка заказа)

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