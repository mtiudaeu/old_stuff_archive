#ifndef MAT_UTILS_LOGGE_LOG_HPP
#define MAT_UTILS_LOGGE_LOG_HPP

#include <iostream>

// Declarations
namespace mat_utils
{
   class logger
   {
      public:
      enum debug_level
      {
         OFF = 0,
         ERROR,
         WARNING,
         INFO,
         DEBUG
      };

      static debug_level get_level();
      static void set_level(debug_level level);

      static bool is_active(debug_level level);

      private:
      debug_level m_level;

      logger();

      static logger& get_instance();
   };
}

// Implementations
mat_utils::logger::logger() :
   m_level(DEBUG)
{
}

mat_utils::logger& mat_utils::logger::get_instance()
{
   static logger logger_instance;
   return logger_instance;
}

mat_utils::logger::debug_level mat_utils::logger::get_level()
{
   return get_instance().m_level;
}

void mat_utils::logger::set_level(debug_level level)
{
   get_instance().m_level = level;
}

bool mat_utils::logger::is_active(mat_utils::logger::debug_level level)
{
   return get_level() >= level;
}

//Macros
#define LOG_ERROR(a) if(mat_utils::logger::is_active(mat_utils::logger::ERROR)) { std::cerr << "ERROR: " << a << " : " << __FILE__ << ":" << __LINE__ << std::endl; }
#define LOG_WARNING(a) if(mat_utils::logger::is_active(mat_utils::logger::WARNING)) { std::cerr << "WARNING: " << a << " : " << __FILE__ << ":" << __LINE__ << std::endl; }
#define LOG_INFO(a) if(mat_utils::logger::is_active(mat_utils::logger::INFO)) { std::cout << "INFO: " << a << " : " << __FILE__ << ":" << __LINE__ << std::endl; }
#define LOG_DEBUG(a) if(mat_utils::logger::is_active(mat_utils::logger::DEBUG)) { std::cout << "DEBUG: " << a << " : " << __FILE__ << ":" << __LINE__ << std::endl; }

#endif //MAT_UTILS_LOGGE_LOG_HPP

