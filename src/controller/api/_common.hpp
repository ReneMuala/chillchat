#pragma once
#include "crow/common.h"
#include <list>
#include <string>
#include <map>
#define controller_register_api_route_auth(_controller,_name, _route, _description, _method, _handler) \
      controller::routes[#_controller].push_back({_name, "/api/" #_controller _route, _description, _method, true});\
      CROW_ROUTE(app, "/api/" #_controller _route) \
        .name(_name) \
        .CROW_MIDDLEWARES(app, middleware::auth)\
        .methods(_method)([this](const crow::request &req) {\
          return this->_handler(req);\
    });

#define controller_register_api_route(_controller,_name, _route, _description, _method, _handler) \
      controller::routes[#_controller].push_back({_name, "/api/" #_controller _route, _description, _method, false});\
      CROW_ROUTE(app, "/api/" #_controller _route) \
        .name(_name) \
        .methods(_method)([this](const crow::request &req) {\
          return this->_handler(req);\
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
        static std::map<std::string,std::list<route>> routes;
};
}