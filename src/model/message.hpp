#pragma once

#include <string>
#include <memory>
#include <sqlite_orm/sqlite_orm.h>
#include "crow/json.h"
#include "user.hpp"
#include "channel.hpp"


namespace model {
    struct message {
        int id;
        std::unique_ptr<decltype(model::message::id)> reply_to;
        decltype(model::user::id) user_id;
        decltype(model::channel::id) channel_id;
        std::string content;
        std::string created_at;
        std::string updated_at;

        crow::json::wvalue to_json() const {
            return {
                {"id", id},
                {"user_id", user_id},
                {"channel_id", channel_id},
                {"content", content},
                {"created_at", created_at},
                {"updated_at", updated_at},
                {"reply_to", reply_to ? *reply_to : 0}
            };
        }

        static inline model::message from_json(const crow::json::rvalue & json){
            return {
                json.has("id") ? (int)json["id"] : 0,
                nullptr,
                (int)json["user_id"].i(),
                (int)json["channel_id"].i(),
                json["content"].s(),
            };
        }

        static inline auto make_table() {
            using namespace sqlite_orm;
            return sqlite_orm::make_table("message",
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
