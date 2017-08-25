//
// main.cpp
// ~~~~~~~~
//

#include <iostream>
#include <boost/asio.hpp>
#include "server.hpp"
#include "session_manager.hpp"

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 2)
    {
      std::cerr << "Usage: <port>\n";
      return 1;
    }

    // Initialise the server.
    network::server::server s(argv[1], std::make_shared<network::server::session_manager_echo>());

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
