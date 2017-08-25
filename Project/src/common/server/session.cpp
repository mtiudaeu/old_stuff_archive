//
// session.cpp
// ~~~~~~~~~~~~~~
//

#include "session.hpp"

#include "session_manager.hpp"

#include <utility>
#include <vector>
#include <iostream>

namespace network {
namespace server {

session::session(boost::asio::ip::tcp::socket socket, session_manager& manager)
  : socket_(std::move(socket)),
    session_manager_(manager),
    closed_(false)
{}

session::~session() {
}

void session::start() {
  do_read();
}

void session::close() {
  if (closed_) {
    std::cerr << "session::close : Already closed." << std::endl;
    return;
  }
  closed_ = true;

  //FIXME notification
  auto self(shared_from_this());
  session_manager_.remove(self);

  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  if(ec)
    std::cerr << "session::close shutdown : " << ec.message() << std::endl;

  socket_.close(ec);
  if(ec)
    std::cerr << "session::close close : " << ec.message() << std::endl;
}

void session::write(const std::shared_ptr<std::string> data) {
  const bool write_in_progress = !write_queue_.empty();
  write_queue_.push_back(data);

  if(write_in_progress)
    return;

  auto self(shared_from_this());
  std::function<void()> do_write = [this, self, &do_write]
  {
    boost::asio::async_write(socket_, boost::asio::buffer(*write_queue_.front()),
        [this, self, &do_write](boost::system::error_code ec, std::size_t)
        {
          if (!ec) {
            write_queue_.pop_front();
            const bool write_waiting = !write_queue_.empty();
            if(write_waiting)
              do_write();
          }
          else { close(); }
        });
  };
  do_write();
}

void session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
      [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
      {
        if (!ec) {
          //TODO Use some type of protocol.
          const auto data = std::make_shared<std::string>(
            std::string(std::begin(buffer_), std::begin(buffer_) + bytes_transferred));

          //FIXME Notification instead of using manager.
          session_manager_.receive_msg(data);
          do_read();
        }
        else { close(); }
      });
}

} // namespace server
} // namespace network
