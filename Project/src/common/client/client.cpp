#include <common/client/client.hpp>

#include <boost/bind.hpp>

#include <iostream>
#include <signal.h>
#include <utility>

client::client(const char* host, const char* port)
: io_service_(),
  socket_(io_service_),
  session_manager_(std::bind(&client::handle_read, this, std::placeholders::_1))
{
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::query query(host, port);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator
                                           = resolver.resolve(query);

  boost::asio::async_connect(socket_, endpoint_iterator,
      boost::bind(&client::handle_connect, this,
        boost::asio::placeholders::error));

  thread_.reset(new boost::thread(
      boost::bind(&boost::asio::io_service::run, &io_service_)));
}

client::~client() {
  session_manager_.close();
  if (thread_) thread_->join();
}

void client::write(const char* msg) {
  const std::shared_ptr<std::string> data =
    std::make_shared<std::string>(std::string(msg));
  session_manager_.write_all(data);
}

void client::handle_connect(const boost::system::error_code& ec) {
  if(!ec)
  {
    session_manager_.start(std::make_shared<network::server::session>(
          std::move(socket_), session_manager_));
  }
  else
  {
    std::cerr << "client::handle_connect: " << ec.message() ;
  }
}

void client::handle_read(const std::string& msg) {
  UT_CALL_FUNC( ut_handle_read(msg) ) 
  std::cout << msg << "\n";
}
