#pragma once

#include <memory>
#include <string>
#include <system_error>
#include "../model/channel.hpp"
#include "../model/user_channel.hpp"
#include "../model/user.hpp"
#include "sqlite_orm/sqlite_orm.h"
#include <optional>

namespace service {
    template <typename S>
    class channel {
        S & storage;
        public:
        channel(S & storage) : storage(storage) {}
        int insert(model::channel & channel) {
            channel.created_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
            return channel.id = storage.template insert<model::channel>(channel);
        }

        void update(model::channel & channel) {
            channel.updated_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front();
            storage.template update<model::channel>(channel);
        }

        std::optional<model::channel> get(int id) {
            try {
                return std::move(storage.template get<model::channel>(id));
            } catch (std::system_error & e) {
                return {};
            }
        }

        void remove(const model::channel & channel) {
            storage.template remove<model::channel>(channel);
        }

        int add_user(const model::channel & channel,const model::user & user){
            return storage.template insert<model::user_channel>(model::user_channel {
                .user_id = std::make_unique<int>(user.id),
                .channel_id = std::make_unique<int>(channel.id),
                .created_at = storage.select(sqlite_orm::datetime("now", "+2 hours")).front()
            });
        }

        void remove_user(const model::channel & channel,const model::user & user){
            using namespace  sqlite_orm;
            std::vector<model::user_channel> r = storage.template gel_all<model::user_channel>(sqlite_orm::where(c(&model::user_channel::user_id) == user.id and c(&model::user_channel::channel_id) == channel.id));
            if(not r.empty()){
                storage.template remove<model::user_channel>(r.front());
            }
        }

        int add_channel(const model::channel & channel, model::channel & subchannel){
            subchannel.super = std::make_unique<int>(channel.id);
            update(subchannel);
            return subchannel.id;
        }
    };
}
