#pragma once
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
template <typename... M> class docs {
  crow::Crow<M...> &app;

public:
  docs(crow::Crow<M...> &app)
      :  app(app) {
    controller_register_api_route(docs, "routes", "", "Docs", "GET"_method, get_docs);
 }

  crow::response get_docs(const crow::request &req){
    crow::json::wvalue resp;
    for(const auto & [name, routes] : controller::controller::routes){
      resp[name] = crow::json::wvalue{};
      for(auto& route : routes){
        resp[name][route.name] = crow::json::wvalue{};
        resp[name][route.name]["description"] = route.description;
        resp[name][route.name]["method"] = crow::method_name(route.method);
        resp[name][route.name]["route"] = route.route;
        resp[name][route.name]["auth"] = route.auth ? "required" : "not required";
      }
    }
    return crow::response{resp};
  }
};
} // namespace controller