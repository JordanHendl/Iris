#include "Log.h"
#include <sstream>
#include <string>
#include <ostream>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstring>

namespace karma
{
  namespace log
  {
    void operator<<( String& first, const char* second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }

    void operator<<( String& first, unsigned second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }

    void operator<<( String& first, double second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }
    
    void operator<<( String& first, float second )
    {
      std::stringstream stream ;
      std::string       string ;
      
      stream << first.str() ;
      stream << second      ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }
    
    void operator<<( String& first, const String& second )
    {
      std::stringstream stream ;
      std::string       string ;
      
      stream << first.str()  ;
      stream << second.str() ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }
    
    struct LogData
    {
      const unsigned INITIAL_LOG_SIZE = 4000 ;
      
      std::string    output_path ;
      char*          log         ;
      unsigned       log_size    ;
      unsigned       current_pos ;
      bool           use_stdout  ;
      
      /** Default constructor. Initializes member data.
       */
      LogData() ;
      
      /** Method to convert a log level to a string equivalent.
       * @param level The level of log to convert to string.
       * @return The string value of the log level.
       */
      const char* stringFromLogLevel( Log::Level level ) ;
      
      /**
       * @param append
       */
      void write() ;
    };
    
    struct StringData
    {
      std::string str ;
    };
    
    static LogData log_data ;

    LogData::LogData()
    {
      this->log         = new char[ this->INITIAL_LOG_SIZE ] ;
      this->log_size    = this->INITIAL_LOG_SIZE             ;
      this->use_stdout  = true                               ;
      this->current_pos = 0                                  ;
      

    }

    const char* LogData::stringFromLogLevel( Log::Level level )
    {
      switch( level )
      {
        case Log::Level::Warning : return "WARNING: " ;
        case Log::Level::Fatal   : return "FATAL  : " ;
        case Log::Level::None    : return ""          ;
        default                  : return ""          ; 
      };
    }
    
    void LogData::write()
    {
      std::ofstream out ;
      
      out.open( this->output_path, std::ios::app ) ;

      if( out )
      {
        out.write( log_data.log, log_data.current_pos ) ;
        log_data.current_pos = 0 ;
      }

      out.close() ;
    }

    String::String()
    {
      this->string_data = new StringData() ;
    }

    String::String( const String& string )
    {
      this->string_data = new StringData() ;
      *this->string_data = *string.string_data ;
    }
    
    void String::operator=( const String& string )
    {
      *this->string_data = *string.string_data ;
    }
        
    String::~String()
    {
      delete this->string_data ;
    }
    
    void String::setStr( const char* str )
    {
      data().str = str ;
    }

    const char* String::str() const
    {
      return data().str.c_str() ;
    }

    StringData& String::data()
    {
      return *this->string_data ;
    }

    const StringData& String::data() const
    {
      return *this->string_data ;
    }
    
    void Log::initialize( const char* output, bool use_stdout )
    {
      auto now = std::chrono::system_clock::now() ;

      std::stringstream file_name  ;
      time_t            time       ;
      tm                local_time ;
      
      char ch ;

      log_data.output_path = output     ;
      log_data.use_stdout  = use_stdout ;
      
      ch = log_data.output_path.back() ;
      
      if( ch == '\\' || ch == '/' )
      {
        log_data.output_path.pop_back() ;
      }
      
      time       = std::chrono::system_clock::to_time_t( now ) ;
      local_time = *localtime( &time )                         ;
      
      file_name << "/karma_debug_log_"  ;
      file_name << local_time.tm_mon    ;
      file_name << "_"                  ;
      file_name << local_time.tm_mday   ;
      file_name << "_"                  ;
      file_name << local_time.tm_year   ;
      file_name << "_"                  ;
      file_name << local_time.tm_hour   ;
      file_name << local_time.tm_min    ;
      file_name << local_time.tm_sec    ;
      file_name << ".txt"               ;
      
      log_data.output_path += file_name.str() ;
    }
    
    void Log::write()
    {
      log_data.write() ;
    }

    void Log::outputBase( const char* out, Level level )
    {
      const std::string base_str  = "KARMA_LOGDEBUG::"                   ;
      const std::string level_str = log_data.stringFromLogLevel( level ) ;

      std::string msg ;
      int         cx  ;
      
      msg = base_str + level_str + out ;
      msg.push_back( '\n' ) ;
      
      if( log_data.use_stdout ) std::cout << level_str << msg.data() ;
      
      cx = snprintf( log_data.log + log_data.current_pos, log_data.log_size, msg.data() ) ;
      
      if( cx < 0 || log_data.current_pos + cx > log_data.log_size )
      {
        log_data.write() ;
        std::memset( log_data.log, '0', log_data.log_size ) ;
      }
      else
      {
        log_data.current_pos += cx ;
      }
    }
  }
}
