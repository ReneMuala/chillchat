#pragma once

#include "crow/json.h"
#include <string>
#include <sqlite_orm/sqlite_orm.h>

namespace model{
    struct channel {
        int id;
        std::unique_ptr<decltype(model::channel::id)> super;
        std::string name;
        std::string description;
        std::string language;
        bool is_public;
        std::string created_at;
        std::string updated_at;

        inline crow::json::wvalue to_json() const {
            return {
                {"id", id},
                {"name", name},
                {"description", description},
                {"language", language},
                {"is_public", is_public},
                {"created_at", created_at},
                {"updated_at", updated_at},
                {"super", super ? *super : 0}
            };
        }

        static inline model::channel from_json(const crow::json::rvalue & json){
            return {
                0,
                nullptr,
                json["name"].s(),
                json["description"].s(),
                json["language"].s(),
                json["is_public"].b(),
            };
        }

        static inline auto make_table(){
            using namespace sqlite_orm;
            return sqlite_orm::make_table("channel",
                make_column("id", &channel::id, primary_key().autoincrement()),
                make_column("name", &channel::name),
                make_column("description", &channel::description),
                make_column("language", &channel::language),
                make_column("created_at", &channel::created_at),
                make_column("updated_at", &channel::updated_at),
                make_column("is_public", &channel::is_public),
                make_column("super", &channel::super)
            );
        }
    };
}
