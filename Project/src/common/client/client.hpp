#include <common/server/session_manager.hpp>

#include <common/unit_test/intrusive_unit_test.hpp>

#include <deque>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

class client
{
public:
  /// Construct session for the client side.
  client(const char* host, const char* port);
  ~client();

  /// Write message to the server.
  void write(const char* msg);

  /// FIXME Before we have a client manager.
  boost::asio::io_service& get_io_service() { return io_service_; }
  
  UT_DEF_VIRT_FUNC( void ut_handle_read(const std::string& msg) )

private:

  /// Handle after connection to server.
  void handle_connect(const boost::system::error_code& error);

  /// Handle received data.
  void handle_read(const std::string& msg);

private:
  /// The io_service used to perform asynchronous operations.
  //FIXME should probably be a reference obtained by a client manager.
  boost::asio::io_service io_service_;

  /// Socket for the session
  tcp::socket socket_;

  network::server::session_manager session_manager_;

  std::unique_ptr<boost::thread> thread_;
};
