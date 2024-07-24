#pragma once
#include "crow/common.h"
#include <list>
#include <map>
#include <string>

#define controller_register_api_route_auth(_controller, _name, _route,         \
                                           _description, _method, _handler)    \
  controller::routes[#_controller].push_back(                                  \
      {_name, "/api/" #_controller _route, _description, _method, true});      \
  CROW_ROUTE(app, "/api/" #_controller _route)                                 \
      .name(_name)                                                             \
      .CROW_MIDDLEWARES(app, middleware::auth)                                 \
      .methods(_method)([this](const crow::request &req) {                     \
        try {                                                                  \
          return this->_handler(req);                                          \
        } catch (std::runtime_error & e) {                                     \
          crow::json::wvalue resp;                                             \
          resp["error"] = e.what();                                            \
          return crow::response{crow::status::BAD_REQUEST, resp};              \
        }                                                                      \
      });

#define controller_register_api_route(_controller, _name, _route,              \
                                      _description, _method, _handler)         \
  controller::routes[#_controller].push_back(                                  \
      {_name, "/api/" #_controller _route, _description, _method, false});     \
  CROW_ROUTE(app, "/api/" #_controller _route)                                 \
      .name(_name)                                                             \
      .methods(_method)([this](const crow::request &req) {                     \
        try {                                                                  \
          return this->_handler(req);                                          \
        } catch (std::runtime_error & e) {                                     \
          crow::json::wvalue resp;                                             \
          resp["error"] = e.what();                                            \
          return crow::response{crow::status::BAD_REQUEST, resp};              \
        }                                                                      \
      });

namespace controller {
struct route {
  std::string name;
  std::string route;
  std::string description;
  crow::HTTPMethod method;
  bool auth;
};

class controller {
public:
  static std::map<std::string, std::list<route>> routes;
};
} // namespace controller