#pragma once

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
            
            if(message.channel_id and message.user_id and not storage.template get_all<model::user_channel>(where(c(&model::user_channel::user_id) == *message.user_id and c(&model::user_channel::channel_id) == *message.channel_id)).empty()){
                message.created_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
                return message.id = storage.template insert<model::message>(message);
            } else {
                throw std::system_error(std::make_error_code(std::errc::permission_denied));
            }
        }

        void update(model::message & message) {
            message.updated_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
            storage.template update<model::message>(message);
        }

        std::optional<model::message> get(int id) {
            try {
                return std::move(storage.template get<model::message>(id));
            } catch (std::system_error & e) {
                return {};
            }
        }

        void remove(const model::message & message) {
            storage.template remove<model::message>(message);
        }
    };
}
