# Формат уведомлений

- [cart_changed](#Изменилась-корзина)
- [order_changed](#Изменился-заказ)
- [new_order](#Новый-заказ)
- [menu_changed](#Изменилось-меню)
- ОПЦИОНАЛЬНО: [new_sign_in](#Новая-попытка-входа)

## Изменилась корзина

```json
{
  "event": "cart_changed",
  "timestamp": "2022/01/25 22:35:06"
}
```

## Изменился заказ

(в частности статус)

```json
{
  "event": "order_changed",
  "timestamp": "2022/01/25 22:35:06",
  "body": {
    "order_id": 1
  }
}
```

## Новый заказ

```json
{
  "event": "new_order",
  "timestamp": "2022/01/25 22:35:06",
  "body": {
    "order_id": 1
  }
}
```

## Изменилось меню

```json
{
  "event": "menu_changed",
  "timestamp": "2022/01/25 22:35:06"
}
```

## Новая попытка входа

```json
{
  "event": "new_sign_in",
  "timestamp": "2022/01/25 22:35:06",
  "body": {
    "query": "sign_in",
    "status": 0
  }
}
```