#ifndef BASE_INIT_HPP
#define BASE_INIT_HPP

//MDTMP Add class to handle program init
//MDTMP Add logger initialization.
#include <functional>
#include <map>

class Md_InitProgram
{
public:
  enum ARGS_NB {
    ARGS_NONE,
    ARGS_ONE,
    ARGS_MULTIPLE
  };

private:
  struct Arg {
    std::function<void(const std::string&)> func_;
    ARGS_NB arg_nb_;

    Arg( std::function<void(const std::string&)> func, ARGS_NB arg_nb ) :
      func_(func),
      arg_nb_(arg_nb)
    {}
  };

  std::map<std::string,Arg> args_map_;
  const std::pair<const std::string,Arg>* current_arg_;

public:

  Md_InitProgram();

  bool add( const std::string& arg_name, ARGS_NB arg_nb, std::function<void(const std::string&)> func);
  bool init(int argc, char *argv[]);
  
//MDTMP add printUsage()
};

class Md_DefaultInitProgram : public Md_InitProgram {
public:
  Md_DefaultInitProgram();
};


#endif //BASE_INIT_HPP

