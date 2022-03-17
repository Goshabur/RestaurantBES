# Запуск

## Сервер

По умолчанию `workers=10`
```shell
./Server --port=1234 --SSLkeys=$(pwd)/SSL-keys --workers=50
```

## Клиент

По умолчанию `server=localhost`, `name=Anonymous`
```shell
./Client --name=Alice --port=1234 --server=${domain} # отправка сообщений с клавиатуры
```

```shell
./Client --name=Alice --port=1234 --count_seconds --server=${domain} # сообщения с отсчетом секунд
```

# Библиотеки для установки

* restbed
* folly
* gflags