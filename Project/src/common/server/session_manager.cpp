//
// session_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//

#include "session_manager.hpp"

namespace network {
namespace server {

session_manager::session_manager(std::function<void(const std::string&)> read_callback) :
  read_callback_(read_callback)
{}

void session_manager::start(session_ptr c) {
  //FIXME Thread safety
  sessions_.insert(c);
  c->start();
}

void session_manager::remove(session_ptr c) {
  //FIXME Thread safety
  auto it = sessions_.find(c);
  if (it!=sessions_.end()) {
    sessions_.erase(c);
  }
}

void session_manager::close() {
  for (auto it: sessions_)
    it->close();
}

void session_manager::write_all(const std::shared_ptr<std::string> data) {
  //FIXME Thread safety
  for (auto w: sessions_)
    w->write(data);
}

void session_manager::receive_msg(const std::shared_ptr<std::string> data) {
  if (read_callback_)
    read_callback_(*data);
}

session_manager_echo::session_manager_echo(std::function<void(const std::string&)> read_callback) :
  session_manager(read_callback)
{}

void session_manager_echo::receive_msg(const std::shared_ptr<std::string> data) {
  session_manager::receive_msg(data);
  write_all(data);
}

} // namespace server
} // namespace network
