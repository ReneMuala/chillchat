#pragma once
#include "crow/json.h"
#include <string>
#include <memory>
#include <sqlite_orm/sqlite_orm.h>
namespace model {
    struct user {
        int id;
        std::unique_ptr<decltype(model::user::id)> super;
        std::string name;
        std::string email;
        std::string password;
        std::string created_at;
        std::string updated_at;

        inline crow::json::wvalue to_json() const {
            return {
                {"id", id},
                {"name", name},
                {"email", email},
            };
        }


        static inline model::user from_json(const crow::json::rvalue & json){
            return {
                0,
                nullptr,
                json["name"].s(),
                json["email"].s(),
                json["password"].s(),
            };
        }
        
        static inline auto make_table() {
            using namespace sqlite_orm;
            return sqlite_orm::make_table("user",
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
