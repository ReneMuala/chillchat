#pragma once

#include <string>
#include <memory>
#include <sqlite_orm/sqlite_orm.h>
#include "user.hpp"
#include "channel.hpp"

namespace model {
    struct message {
        int id;
        std::unique_ptr<int> reply_to;
        std::unique_ptr<int> user_id;
        std::unique_ptr<int> channel_id;
        std::string content;
        std::string created_at;
        std::string updated_at;

        static inline auto make_table() {
            using namespace sqlite_orm;
            return sqlite_orm::make_table("messages",
                make_column("id", &message::id,primary_key(). autoincrement()),
                make_column("user_id", &message::user_id),
                make_column("channel_id", &message::channel_id),
                make_column("content", &message::content),
                make_column("created_at", &message::created_at),
                make_column("updated_at", &message::updated_at),
                make_column("reply_to", &message::reply_to),
                foreign_key(&message::user_id).references(&model::user::id),
                foreign_key(&message::channel_id).references(&model::channel::id)
            );
        }
    };
}
