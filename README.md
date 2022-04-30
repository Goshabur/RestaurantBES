# RestaurantBES
HSE first year project

# Клиент-сервер
![Клиент-Сервер](https://github.com/Goshabur/RestaurantBES/blob/main/Client-Server%20Model.png)

# Схема БД
![Схема БД](https://github.com/Goshabur/RestaurantBES/blob/main/DataBase%20UML.png)

# Сервер и взаимодействие с БД
![Сервер](https://github.com/Goshabur/RestaurantBES/blob/main/Server%20UML.png)

# Схема сетей
![Сети](https://github.com/Goshabur/RestaurantBES/blob/main/Network_diagram.png)

# Команды
### Тип 1:
    {...,
    command : "create_order",
    user_id : "00001",
    ...}
- create_order
- empty_cart
- show_cart
### Тип 2:
    {...,
    command : "add_to_cart",
    user_id : "00001",
    product_id : "03",
    ...}
- add_to_cart
- delete_from_cart
### Тип 3:
    {...,
    command : "sign_in",
    email : "bes@mail.ru",
    password : "something",
    name: "Alice"  // только для sign_up
    ...}
- sign_in
- sign_up
### Тип 4 (админ):
    {...,
    command : "change_dish_price",
    product_id : "03",
    new_value : "100",
    ...}
- change_dish_price
- change_dish_status
- change_dish_info

 






















