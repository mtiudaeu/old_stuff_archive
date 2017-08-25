//
// server.hpp
// ~~~~~~~~~~

#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "session_manager.hpp"

namespace network {
namespace server {

class server
{
public:
  server(const server&) = delete;
  server(server&&) = delete;
  server& operator=(const server&) = delete;

  /// Construct the server to listen on the specified TCP address and port
  server(const std::string& port,
         std::shared_ptr<session_manager> manager);

  /// Run the server's io_service loop.
  void run();

private:
  /// Perform an asynchronous accept operation.
  void do_accept();

  /// Wait for a request to stop the server.
  void do_await_stop();

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set signals_;

  /// Acceptor used to listen for incoming sessions.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The session manager which owns all live sessions.
  std::shared_ptr<session_manager> session_manager_;

  /// The next socket to be accepted.
  boost::asio::ip::tcp::socket socket_;
};

} // namespace server
} // namespace network

#endif // SERVER_HPP
