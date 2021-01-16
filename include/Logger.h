#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdint.h>
#include <string>
#include <syslog.h>

class Logger
{
public:
  explicit Logger( const std::string& logName ) : mLogName( logName )
  {
    if( logInstancesCounter == 0 )
    {
      openlog( "HttpServer", LOG_CONS | LOG_PID | LOG_NDELAY | LOG_PERROR, LOG_USER );
    }

    ++logInstancesCounter;
  }

  ~Logger()
  {
    --logInstancesCounter;
    if( logInstancesCounter <= 0 )
    {
      closelog();
    }
  }

  template<class... Args>
  void error( const char* msg, Args... args )
  {
    syslog( LOG_ERR, std::string( '<' + mLogName + "> ERROR: " ).append( msg ).c_str(), args... );
  }

  template<class... Args>
  void warning( const char* msg, Args... args )
  {
    syslog( LOG_WARNING, std::string( '<' + mLogName + "> WARNING: " ).append( msg ).c_str(), args... );
  }

  template<class... Args>
  void info( const char* msg, Args... args )
  {
    syslog( LOG_INFO, std::string( '<' + mLogName + "> INFO: " ).append( msg ).c_str(), args... );
  }

  template<class... Args>
  void debug( const char* msg, Args... args )
  {
    syslog( LOG_DEBUG, std::string( '<' + mLogName + "> DEBUG: " ).append( msg ).c_str(), args... );
  }

private:
  static int32_t logInstancesCounter;
  std::string    mLogName;
};

#endif // _LOGGER_H_
