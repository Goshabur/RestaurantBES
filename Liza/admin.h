#pragma once

#include "client.h"

namespace restbes {
    class Admin {
    public:
        Admin() = default;

        static void change_order_status(int order_id, int set_status) noexcept;

        static void cancel_order(int order_id) noexcept;

        static void accept_order(restbes::Client &user) noexcept;
    };
}  // namespace restbes
