#include <iostream>
#include <optional>
#include "model/model.hpp"
#include "model/user.hpp"
#include "service/channel.hpp"
#include "service/user.hpp"
#include "sqlite_orm/sqlite_orm.h"

int main(int argc, char** argv) {
    auto s = model::make_storage();
    auto us = service::user(s);
    auto cs = service::channel(s);
    s.remove_all<model::user>();
    s.remove_all<model::channel>();
    auto user = model::user {
        .email = "rene@gmail.com",
        .password = "1234",
        .name = "René",
     };
    us.insert(user);
    auto channel = model::channel {
        .name = "landia",
        .description = "landia channel",
        .language = "pt"
    };
    cs.insert(channel);

    // sqlite_orm::make_trigger("set_created_at", sqlite_orm::after().insert().on<model::user>().begin(sqlite_orm::update(where(old(&model::user::id)))))
    for(auto & u : s.get_all<model::user>()){
        std::cout << u.name << " " << u.created_at << std::endl;
    }
    // auto u = us.get_login("rene@gmail.com", "1234");
    // if (u) {
    //     std::cout << "user: " << u->email << std::endl;
    //     std::cout << "created_at: " << u->created_at << std::endl;

    // } else {
    //     std::cout << "user not found" << std::endl;
    // }
    return 0;
}
