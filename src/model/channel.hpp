#pragma once

#include <string>
#include <sqlite_orm/sqlite_orm.h>

namespace model{
    struct channel {
        int id;
        std::unique_ptr<int> super;
        std::string name;
        std::string description;
        std::string language;
        std::string created_at;
        std::string updated_at;

        static inline auto make_table(){
            using namespace sqlite_orm;
            return sqlite_orm::make_table("channel",
                make_column("id", &channel::id, primary_key().autoincrement()),
                make_column("name", &channel::name),
                make_column("description", &channel::description),
                make_column("language", &channel::language),
                make_column("created_at", &channel::created_at),
                make_column("updated_at", &channel::updated_at),
                make_column("super", &channel::super)
            );
        }
    };
}
