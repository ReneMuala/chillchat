#pragma once

#include <string>
#include <system_error>
#include "../model/message.hpp"
#include "../model/user_channel.hpp"

namespace service {
    template <typename S>
    class message {
        S & storage;
        public:
        message(S & storage) : storage(storage) {}
        int insert(model::message & message) {
            using namespace sqlite_orm;
            if(storage.template select<model::user_channel>(where(c(&model::user_channel::user_id) == message.user_id.get() and c(&model::user_channel::channel_id) == message.channel_id.get()))){
                message.created_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
                return message.id = storage.template insert(message);
            }
        }

        void update(model::message & message) {
            message.updated_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
            storage.template update<model::message>(message);
        }

        std::optional<model::message> get(int id) {
            try {
                return storage.template get<model::message>(id);
            } catch (std::system_error & e) {
                return {};
            }
        }

        void remove(const model::message & message) {
            storage.template remove<model::message>(message);
        }
    };
}
