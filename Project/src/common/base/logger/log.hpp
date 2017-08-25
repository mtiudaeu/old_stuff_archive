#ifndef BASE_LOG_HPP
#define BASE_LOG_HPP

#include <iostream>

namespace Md_Logger
{
  bool isActive();
  void setActive(bool active);
}

#define LOG_ERROR(a) if(Md_Logger::isActive()) { std::cerr << __FILE__ << ":" << __LINE__ << " : " << a << std::endl; }

#endif //BASE_LOG_HPP

