#pragma once
#include "../../middleware/auth.hpp"
#include "../../service/user.hpp"
#include "_common.hpp"
#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "crow/utility.h"
#include <crow/app.h>
#include <optional>
#include <stdexcept>

namespace controller {
using Session = crow::SessionMiddleware<crow::FileStore>;
template <typename S, typename... M> class user : public controller {
  crow::Crow<M...> &app;
  service::user<S> &service;

public:
  user(crow::Crow<M...> &app, service::user<S> &service)
      : service(service), app(app) {
    controller_register_api_route_auth(
        user, "profile", "/profile", "User profile", "GET"_method, get_profile);
    controller_register_api_route_auth(
        user, "channels", "/channels",
        "A list of channels in whitch you are a member", "GET"_method,
        get_channels);
    controller_register_api_route(user, "create", "/create", "Create a new user",
                                  "POST"_method, create);
    controller_register_api_route_auth(user, "update", "/update", "Create a new user",
                                  "PUT"_method, update);
  }

  crow::response create(const crow::request &req) {
    crow::json::rvalue body = crow::json::load(req.body);
    auto user = model::user::from_json(body);
    auto id = service.insert(user);
    return crow::response{user.to_json()};
  }

  crow::response update(const crow::request &req){
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};
    crow::json::rvalue body = crow::json::load(req.body);
    
    auto new_user = model::user::from_json(body);
    new_user.id = user.value().id;
    new_user.created_at = user.value().created_at;
    service.update(new_user);
    return crow::response{new_user.to_json()};
  }


  crow::response get_profile(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};
    return crow::response{user.value().to_json()};
  }

  crow::response get_channels(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};
    
    auto channels = service.get_channels(user.value());

    auto json = crow::json::wvalue{};
    int i = 0;
    for (auto &channel : channels)
      json[i++] = channel.to_json();

    return crow::response{json};
  }
};
} // namespace controller