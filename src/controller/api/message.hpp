#pragma once
#include "../../middleware/auth.hpp"
#include "../../service/message.hpp"
#include "../../service/user.hpp"
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
template <typename US, typename MS, typename... M>
class message : public controller {
  crow::Crow<M...> &app;
  service::user<US> &user_service;
  service::message<MS> &message_service;

public:
  message(crow::Crow<M...> &app, service::user<US> &user_service,
          service::message<MS> &message_service)
      : app(app), user_service(user_service), message_service(message_service) {
    controller_register_api_route_auth(
        message, "send", "/send", "send a new message", "POST"_method, profile);
        
  }

  crow::response profile(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = user_service.get(id);
     if (!user)
      return crow::response{crow::status::FORBIDDEN};

    auto resp = crow::json::wvalue{};
    crow::json::wvalue body = crow::json::load(req.body);

    try {
      body["user_id"] = user.value().id;
      auto message = model::message::from_json(crow::json::load(body.dump()));
      message_service.insert(message);
      resp["message_id"] = message.id;
      return crow::response{resp};
    } catch (std::runtime_error &e) {
      resp["error"] = e.what();
      return crow::response{crow::status::BAD_REQUEST, resp};
    }
  }
};
} // namespace controller