//
// server.cpp
// ~~~~~~~~~~
//

#include "server.hpp"

#include <signal.h>
#include <utility>

namespace network {
namespace server {

server::server(const std::string& port,
               std::shared_ptr<session_manager> manager)
  : io_service_(),
    signals_(io_service_),
    acceptor_(io_service_),
    session_manager_(manager),
    socket_(io_service_)
{
  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  signals_.add(SIGINT);
  signals_.add(SIGTERM);
#if defined(SIGQUIT)
  signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

  do_await_stop();

  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), atoi(port.c_str()));  
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  do_accept();
}

void server::run()
{
  // The io_service::run() call will block until all asynchronous operations
  // have finished. While the server is running, there is always at least one
  // asynchronous operation outstanding: the asynchronous accept call waiting
  // for new incoming sessions.
  io_service_.run();
}

void server::do_accept()
{
  acceptor_.async_accept(socket_,
      [this](boost::system::error_code ec)
      {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open())
        {
          return;
        }

        if (!ec)
        {
          session_manager_->start(std::make_shared<session>(
              std::move(socket_), *session_manager_));
        }

        do_accept();
      });
}

void server::do_await_stop()
{
  signals_.async_wait(
      [this](boost::system::error_code /*ec*/, int /*signo*/)
      {
        // The server is stopped by cancelling all outstanding asynchronous
        // operations. Once all operations have finished the io_service::run()
        // call will exit.
        acceptor_.close();
        session_manager_->close();
      });
}

} // namespace server
} // namespace network

