#pragma once

#include <string>
#include <system_error>
#include "../model/user.hpp"
#include "../model/channel.hpp"
#include "../model/user_channel.hpp"
#include <optional>

namespace service {
    template <typename S>
    class user {
        S & storage;
        public:
        user(S & storage) : storage(storage) {}
        int insert(model::user & user) {
            user.created_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
            return user.id = storage.template insert<model::user>(user);
        }

        void update(model::user & user) {
            user.updated_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
            storage.template update<model::user>(user);
        }

        std::vector<model::channel> get_channels(const model::user & user){
            using namespace sqlite_orm;
            return storage.template get_all<model::channel>(
                where(
                    in(&model::channel::id, 
                    select(&model::user_channel::channel_id, 
                        where(c(&model::user_channel::user_id) == user.id)))));
        }

        std::optional<model::user> get(int id) {
            try {
                return std::move(storage.template get<model::user>(id));
            } catch (std::system_error & e) {
                return {};
            }
        }

        std::optional<model::user> get_login(const std::string & email, const std::string & password) {
            using namespace sqlite_orm;
            try {
                return std::move(storage.template get_all<model::user>(where(c(&model::user::email) == email and c(&model::user::password) == password)).at(0));
            } catch (std::system_error & e) {
                return {};
            } catch (std::out_of_range & e) {
                return {};
            }
        }

        void remove(const model::user & user) {
            storage.template remove<model::user>(user);
        }

        int add_user(const model::user & user, model::user & subuser){
            subuser.super = std::make_unique<int>(user.id);
            update(subuser);
            return subuser.id;
        }
    };
}
