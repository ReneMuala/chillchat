#pragma once
#include "../middleware/auth.hpp"
#include "../service/message.hpp"
#include "../service/user.hpp"
#include "_common.hpp"
#include "crow/TinySHA1.hpp"
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
template <typename US, typename MS, typename... M>
class message : public controller {
  crow::Crow<M...> &app;
  service::user<US> &user_service;
  service::message<MS> &message_service;

public:
  message(crow::Crow<M...> &app, service::user<US> &user_service,
          service::message<MS> &message_service)
      : app(app), user_service(user_service), message_service(message_service) {
    controller_register_api_route_auth(message, "create", "/create",
                                       "send a new message", "POST"_method,
                                       create);
    controller_register_api_route_auth(message, "update", "/update",
                                       "updates a message", "PUT"_method,
                                       update);
    controller_register_api_route_auth(
        message, "all", "/all",
        "get path(limit) messages created or updated after path(after)", "GET"_method,
        all);
  }
  
  crow::response create(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = user_service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};

    crow::json::wvalue body = crow::json::load(req.body);

    body["user_id"] = user.value().id;
    auto message = model::message::from_json(crow::json::load(body.dump()));
    message_service.insert(message);
    return crow::response{message.to_json()};
  }

  crow::response update(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = user_service.get(id);
    if (!user)
      return crow::response{crow::status::FORBIDDEN};
    crow::json::wvalue body = crow::json::load(req.body);
    body["user_id"] = user.value().id;
    auto new_message = model::message::from_json(crow::json::load(body.dump()));
    auto old_message = message_service.get(new_message.id);
    if (!old_message or old_message.value().user_id != user.value().id)
      return crow::response{crow::status::NOT_FOUND};
    new_message.id = old_message.value().id;
    new_message.created_at = old_message.value().created_at;
    message_service.update(new_message);
    return crow::response{new_message.to_json()};
  }
};
} // namespace controller