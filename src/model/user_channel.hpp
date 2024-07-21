#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>
#include "user.hpp"
#include "channel.hpp"

namespace model {
    struct user_channel {
        int id;
        std::unique_ptr<int> user_id;
        std::unique_ptr<int> channel_id;
        bool is_admin;
        int last_readed_message;
        std::string created_at;

        static inline auto make_table() {
            using namespace sqlite_orm;
            return sqlite_orm::make_table("users",
                make_column("id", &user_channel::id, primary_key().autoincrement()),
                make_column("user_id", &user_channel::user_id, unique()),
                make_column("channel_id", &user_channel::channel_id, unique()),
                make_column("is_admin", &user_channel::is_admin),
                make_column("last_readed_message", &user_channel::last_readed_message),
                foreign_key(&user_channel::user_id).references(&model::user::id),
                foreign_key(&user_channel::channel_id).references(&model::channel::id)
            );
        }
    };
}
