//
// session_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "session.hpp"

#include <set>
#include <functional>

namespace network {
namespace server {

/// Manages open sessions so that they may be cleanly stopped when the server
/// needs to shut down.
class session_manager
{
public:
  /// Construct a session manager.
  explicit session_manager(std::function<void(const std::string&)> read_callback =
      std::function<void(const std::string&)>());

  /// Add the specified session to the manager and start it.
  void start(session_ptr c);

  /// Remove the specified session.
  void remove(session_ptr c);

  void close();

  /// Deliver msg to all sessions
//FIXME Should add weak_ptr session parameter.
//      Must be able to filter out one session on demand.
  void write_all(const std::shared_ptr<std::string> data);

  /// Handle message reception from a session.
  virtual void receive_msg(const std::shared_ptr<std::string> data);

private:
  /// The managed sessions.
  std::set<session_ptr> sessions_;

  /// Callback on message received.
  std::function<void(const std::string&)> read_callback_;

private: //Prohibited
  session_manager(const session_manager&) = delete;
  session_manager(session_manager&&) = delete;
  session_manager& operator=(const session_manager&) = delete;
};

class session_manager_echo : public session_manager
{
public:
  session_manager_echo(std::function<void(const std::string&)> read_callback =
      std::function<void(const std::string&)>());

  virtual void receive_msg(const std::shared_ptr<std::string> data) override;

private: //Prohibited
  session_manager_echo(const session_manager_echo&) = delete;
  session_manager_echo(session_manager_echo&&) = delete;
  session_manager_echo& operator=(const session_manager_echo&) = delete;
};

//TODO Add Derived class for echo to other participants


} // namespace server
} // namespace network

#endif // CONNECTION_MANAGER_HPP
