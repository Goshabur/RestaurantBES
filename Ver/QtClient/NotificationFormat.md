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