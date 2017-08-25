//
// session.hpp
// ~~~~~~~~~~~~~~
//

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <deque>

namespace network {
namespace server {

class session_manager;

/// Represents a single session from a client.
class session
  : public std::enable_shared_from_this<session>
{
public:
  /// Construct a session with the given socket.
   session(boost::asio::ip::tcp::socket socket,
           session_manager& manager);
   ~session();

  /// Start the first asynchronous operation for the session.
  void start();

  /// Stop all asynchronous operations associated with the session.
  void close();

  /// Perform an asynchronous write operation.
  void write(const std::shared_ptr<std::string> data);

private:
  /// Perform an asynchronous read operation.
  void do_read();

  /// Socket for the session.
  boost::asio::ip::tcp::socket socket_;

  /// The manager for this session.
//FIXME session should not know about manager
  session_manager& session_manager_;

private:
  /// Buffer for incoming data.
  std::array<char, 8192> buffer_;

  /// Queue of data to write.
  // FIXME These string should be const
  std::deque<std::shared_ptr<std::string>> write_queue_;

  bool closed_;

private: // Prohibited
  session(const session&) = delete;
  session(session&&) = delete;
  session& operator=(const session&) = delete;
};

typedef std::shared_ptr<session> session_ptr;

} // namespace server
} // namespace network

#endif // CONNECTION_HPP
