#pragma once

#include "client.h"

namespace restbes {

class Admin {
public:
    Admin() = default;

    static void change_order_status(id_t order_id, int set_status) noexcept;

    static void cancel_order(id_t order_id) noexcept;

    static void accept_order(id_t order_id) noexcept;
};
}  // namespace restbes
