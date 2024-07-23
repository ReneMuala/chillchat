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