#include "controller/api/_common.hpp"
#include "controller/api/auth.hpp"
#include "controller/api/docs.hpp"
#include "controller/api/message.hpp"
#include "controller/api/user.hpp"
#include "crow/app.h"
#include "crow/logging.h"
#include "middleware/auth.hpp"
#include "model/message.hpp"
#include "model/model.hpp"
#include "model/user.hpp"
#include "model/user_channel.hpp"
#include "service/channel.hpp"
#include "service/message.hpp"
#include "service/user.hpp"
#include "sqlite_orm/sqlite_orm.h"
#include <crow/middlewares/session.h>
#include <iostream>
#include <optional>

using Session = crow::SessionMiddleware<crow::FileStore>;

void run() {
  auto s = model::make_storage();
  auto us = service::user(s);
  auto cs = service::channel(s);
  auto ms = service::message(s);
//   s.remove_all<model::user_channel>();
//   s.remove_all<model::message>();
//   s.remove_all<model::user>();
//   s.remove_all<model::channel>();
//   auto user = model::user{
//       .name = "René",
//       .email = "rene@gmail.com",
//       .password = "1234",
//   };
//   auto user2 = model::user{
//       .name = "Descartes",
//       .email = "descartes@gmail.com",
//       .password = "1234",
//   };
//   us.insert(user);
//   us.insert(user2);
//   auto channel = model::channel{
//       .name = "landia", .description = "landia channel", .language = "pt"};
//   cs.insert(channel);
//   cs.add_user(channel, user);
//   cs.add_user(channel, user2);
//   using namespace sqlite_orm;

  crow::App<crow::CookieParser, middleware::auth, Session> app{
      Session{
          crow::FileStore{"./"},
      },
  };
  controller::auth auth_controller(app, us);
  controller::user user_controller(app, us);
  controller::docs docs_controller(app);
  controller::message message_controller(app, us, ms);
  app.bindaddr("0.0.0.0").multithreaded().port(8080).run();
}

int main(int argc, char **argv) {
  try {
    run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
