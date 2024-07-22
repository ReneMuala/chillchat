#pragma once
#include "../../service/user.hpp"
#include "crow/common.h"
#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "crow/utility.h"
#include <crow/app.h>
#include <optional>
#include <stdexcept>
#include "../../middleware/auth.hpp"
#include "_common.hpp"

namespace controller {
  using Session = crow::SessionMiddleware<crow::FileStore>;
template <typename S, typename... M> class user : public controller {
  crow::Crow<M...> &app;
  service::user<S> &service;

public:
  user(crow::Crow<M...> &app, service::user<S> &service)
      : service(service), app(app) {
    controller_register_api_route_auth(user, "profile", "/profile", "User profile", "GET"_method, profile);
 }

  crow::response profile(const crow::request &req){
    auto& session = app.get_context<Session>(req);
    auto id = session.get("id", -1);
    auto user = service.get(id);
    crow::json::wvalue resp;
    resp["id"] = id;
    resp["name"] = user->name;
    resp["email"] = user->email;
    return crow::response{resp};
  }
};
} // namespace controller