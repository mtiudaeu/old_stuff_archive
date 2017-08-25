#include <common/base/init/init.hpp>

#include <common/base/logger/log.hpp>

init_program::init_program() :
  current_arg_(0x0)
{
}

bool init_program::add( const std::string& arg_name,
                          ARGS_NB arg_nb,
                          std::function<void(const std::string&)> func)
{
  const auto lb = args_map_.lower_bound(arg_name);
  if(lb != args_map_.end() && lb->first == arg_name) {
    LOG_ERROR("Argument is already in use.");
    return false;
  }

  args_map_.insert(lb, std::make_pair(arg_name, arg(func, arg_nb)));
  return true;
}

bool init_program::init(int argc, char *argv[])
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

