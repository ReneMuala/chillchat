#pragma once
#include "../../middleware/auth.hpp"
#include "../../service/user.hpp"
#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "crow/utility.h"
#include <crow/app.h>
#include <functional>
#include <optional>
#include <stdexcept>
#include "_common.hpp"


namespace controller {
using Session = crow::SessionMiddleware<crow::FileStore>;
template <typename S, typename... M> class auth : public controller {
  crow::Crow<M...> &app;
  service::user<S> &service;

public:

  auth(crow::Crow<M...> &app, service::user<S> &service)
      : service(service), app(app) {
    
    controller_register_api_route(auth, "login", "/login", "Session login", "POST"_method, login);
    controller_register_api_route_auth(auth,"logout", "/logout", "Session logout", "POST"_method, logout);
  }

  crow::response logout(const crow::request &req) {
    auto &session = app.get_context<Session>(req);
    session.set("id", -1);
    session.set("email", "");
    return crow::response{crow::status::OK};
  }

  crow::response login(const crow::request &req) {
    try {
      auto body = crow::json::load(req.body);
      auto email = (std::string)body["email"];
      auto password = (std::string)body["password"];
      std::optional<model::user> user = service.get_login(email, password);
      if (user) {
        auto &session = app.get_context<Session>(req);
        session.set("id", user->id);
        session.set("email", user->email);
        return crow::response{user->email};
      } else {
        return crow::response{crow::status::NOT_FOUND};
      }
    } catch (std::runtime_error &e) {
      return crow::response{crow::status::BAD_REQUEST, e.what()};
    }
  }
};
} // namespace controller