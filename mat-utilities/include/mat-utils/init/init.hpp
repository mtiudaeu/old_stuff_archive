#ifndef MAT_UTILS_INIT_INIT_HPP
#define MAT_UTILS_INIT_INIT_HPP

#include <mat-utils/logger/log.hpp>

#include <functional>
#include <map>

// Declarations
namespace mat_utils
{
class init_program
{
public:
  enum ARGS_NB {
    ARGS_NONE,
    ARGS_ONE,
    ARGS_MULTIPLE
  };

private:
  struct arg {
    std::function<void(const std::string&)> func_;
    ARGS_NB arg_nb_;

    arg( std::function<void(const std::string&)> func, ARGS_NB arg_nb ) :
      func_(func),
      arg_nb_(arg_nb)
    {}
  };

  std::map<std::string,arg> args_map_;
  const std::pair<const std::string,arg>* current_arg_;

public:

  init_program();

  bool add( const std::string& arg_name, ARGS_NB arg_nb, std::function<void(const std::string&)> func);
  bool parse(int argc, const char* argv[]);
};
} //namespace mat_utils

// Implementations
mat_utils::init_program::init_program() :
  current_arg_(0x0)
{
}

bool mat_utils::init_program::add( 
      const std::string& arg_name,
      ARGS_NB arg_nb,
      std::function<void(const std::string&)> func
      )
{
  const auto lb = args_map_.lower_bound(arg_name);
  if(lb != args_map_.end() && lb->first == arg_name) {
    LOG_ERROR("Argument is already in use.");
    return false;
  }

  args_map_.insert(lb, std::make_pair(arg_name, arg(func, arg_nb)));
  return true;
}

bool mat_utils::init_program::parse(int argc, const char* argv[])
{
  for(int i = 1; i < argc; ++i) {
    if(!argv[i]) { LOG_ERROR("Encountered null argument."); return false; }

    const std::string arg = argv[i];
    if(arg[0] == '-') {
      const auto& it = args_map_.find(arg.substr(1, arg.size()-1));
      if(it == args_map_.end()) { LOG_ERROR("Encountered invalid argument : " << arg); return false; }
      current_arg_ = &*it;

      if(current_arg_->second.arg_nb_ == ARGS_NONE) {
        current_arg_->second.func_(arg);
        current_arg_ = 0x0;
      }
    }
    else {
      if(!current_arg_) { LOG_ERROR("Error interpreting argument value : " << arg); return false; }

      current_arg_->second.func_(arg);

      if(current_arg_->second.arg_nb_ == ARGS_ONE) {
        current_arg_ = 0x0;
      }
    }
  }

  return true;
}


#endif //MAT_UTILS_INIT_INIT_HPP

