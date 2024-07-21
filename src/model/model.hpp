#pragma once

#include <string>
#include "channel.hpp"
#include "sqlite_orm/sqlite_orm.h"
#include "user.hpp"
#include "message.hpp"
#include "user_channel.hpp"

namespace model {
    inline auto make_storage(const std::string & path = "chilldb.sqlite"){
        static auto storage_inited = false;
        static auto storage = sqlite_orm::make_storage(path,
            user::make_table(),
            channel::make_table(),
            message::make_table(),
            user_channel::make_table()
        );

        if(!storage_inited){
            storage.sync_schema();
            storage_inited = true;
        }

        return storage;
    }
}
