#include <common/base/logger/log.hpp>

namespace
{
  bool logActive = true;
}

bool Md_Logger::isActive()
{
  return logActive;
}

void Md_Logger::setActive(bool active)
{
  logActive = active;
}

