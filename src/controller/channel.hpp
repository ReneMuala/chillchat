#pragma once
#include "../middleware/auth.hpp"
#include "../service/channel.hpp"
#include "../service/user.hpp"
#include "_common.hpp"
#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "crow/utility.h"
#include <crow/app.h>
#include <memory>
#include <optional>
#include <stdexcept>


namespace controller {
using Session = crow::SessionMiddleware<crow::FileStore>;
template <typename US, typename CS, typename... M>
class channel : public controller {
  crow::Crow<M...> &app;
  service::user<US> &user_service;
  service::channel<CS> &channel_service;

public:
  channel(crow::Crow<M...> &app, service::user<US> &user_service,
          service::channel<CS> &channel_service)
      : app(app), user_service(user_service), channel_service(channel_service) {
    controller_register_api_route_auth(channel, "create", "/create",
                                       "creates a new channel", "POST"_method,
                                       create);
    controller_register_api_route_auth(channel, "update", "/update",
                                       "updates a channel", "PUT"_method,
                                       update);
  }

  crow::response create(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = user_service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};

    auto body = crow::json::load(req.body);
    auto channel = model::channel::from_json(body);
    channel_service.insert(channel, user.value());
    return crow::response{channel.to_json()};
  }

  crow::response update(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = user_service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};
    crow::json::rvalue body = crow::json::load(req.body);
    auto new_channel = model::channel::from_json(body);
    auto old_channel = channel_service.get(new_channel.id);
    if (!old_channel)
      return crow::response{crow::status::NOT_FOUND};
    new_channel.id = old_channel.value().id;
    new_channel.created_at = old_channel.value().created_at;
    channel_service.update(new_channel, user.value());
    return crow::response{new_channel.to_json()};
  }
};
} // namespace controller