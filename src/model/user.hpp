#pragma once
#include <string>
#include <memory>
#include <sqlite_orm/sqlite_orm.h>
namespace model {
    struct user {
        int id;
        std::unique_ptr<int> super;
        std::string name;
        std::string email;
        std::string password;
        std::string created_at;
        std::string updated_at;

        static inline auto make_table() {
            using namespace sqlite_orm;
            return sqlite_orm::make_table("users",
                make_column("name", &user::name),
                make_column("email", &user::email),
                make_column("password", &user::password),
                make_column("id", &user::id, primary_key().autoincrement()),
                make_column("created_at", &user::created_at, default_value(date("now"))),
                make_column("updated_at", &user::updated_at),
                make_column("super", &user::super)
            );
        }
    };
}
