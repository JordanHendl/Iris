/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Log.h"
#include <sstream>
#include <string>
#include <ostream>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <condition_variable>
#include <time.h>

namespace iris
{
  namespace log
  {
    bool Log::enabled ;

    void operator<<( String& first, char second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }
    
    void operator<<( String& first, const char* second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }

    void operator<<( String& first, unsigned long long second )
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
    
    void operator<<( String& first, bool second )
    {
      std::stringstream stream ;
      std::string       string ;
      
      stream << first.str() ;
      
      if( second ) stream << "true"  ;
      else         stream << "false" ;
      
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
      const unsigned INITIAL_LOG_SIZE = 8000 ;
      tm*            local_time  ;
      std::string    output_path ;
      char*          log         ;
      unsigned       log_size    ;
      unsigned       current_pos ;
      bool           use_stdout  ;
      Log::Mode      mode        ;
      
      /** Default constructor. Initializes member data.
       */
      LogData() ;
      
      /** Method to convert a log level to a string equivalent.
       * @param level The level of log to convert to string.
       * @return The string value of the log level.
       */
      const char* stringFromLogLevel( Log::Level level ) ;
      
      /** Method to create a timestamp from the current time.
       * @return A string representation of the current time.
       */
      std::string timestamp() ;
      
      /** Method to write log out to disk.
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
      this->mode        = Log::Mode::Quiet                   ;
      this->local_time  = nullptr                            ;
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
    
    std::string LogData::timestamp()
    {
      static std::mutex lock   ;
      std::stringstream stream ;
      std::string       str    ;
      
      lock.lock() ;
      if( log_data.local_time == nullptr )
      {
        auto time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() ) ;
        log_data.local_time = localtime( &time )                                             ;
      }
      
      stream << log_data.local_time->tm_hour << "h " << log_data.local_time->tm_min << "m " << log_data.local_time->tm_sec << "s |" ;
      log_data.local_time = nullptr ;
      lock.unlock() ;
      
      str = std::string( stream.str().c_str() ) ;
      return str ;
    }

    void LogData::write()
    {
      std::ofstream out ;
      
      out.open( this->output_path, std::ios::app ) ;

      if( out.is_open() )
      {
        out.write( log_data.log, log_data.current_pos ) ;
        log_data.current_pos = 0 ;
        out.close() ;
      }
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
      
      
      char ch ;

      log_data.output_path = output     ;
      log_data.use_stdout  = use_stdout ;
      
      ch = log_data.output_path.back() ;
      
      if( ch == '\\' || ch == '/' )
      {
        log_data.output_path.pop_back() ;
      }
      
      time                = std::chrono::system_clock::to_time_t( now ) ;
      log_data.local_time = localtime( &time )                          ;
      
      file_name << "/iris_debug_log_"            ;
      file_name << log_data.local_time->tm_mon   ;
      file_name << "D_"                          ;
      file_name << log_data.local_time->tm_mday  ;
      file_name << "M_"                          ;
      file_name << log_data.local_time->tm_year  ;
      file_name << "Y_"                          ;
      file_name << log_data.local_time->tm_hour  ;
      file_name << "H"                           ;
      file_name << log_data.local_time->tm_min   ;
      file_name << "M"                           ;
      file_name << log_data.local_time->tm_sec   ;
      file_name << "S"                           ;
      file_name << ".txt"                        ;
      
      log_data.local_time = nullptr ;
      log_data.output_path += file_name.str() ;
    }
    
    void Log::flush()
    {
      log_data.write() ;
    }
    
    void Log::setMode( Mode mode )
    {
      log_data.mode = mode ;
    }
    
    void Log::setEnabled( bool value )
    {
      Log::enabled = value ;
    }

    void Log::outputBase( const char* out, Level level )
    {
      const std::string timestamp = log_data.timestamp()                 ;
      const std::string level_str = log_data.stringFromLogLevel( level ) ;
      static std::mutex log_mutex ;
      std::stringstream base_str  ;
      std::stringstream msg       ;
      int               cx        ;
      
      log_mutex.lock() ;
      if( log_data.mode == Log::Mode::Verbose )
      {
        base_str << timestamp ;
      }
      
      base_str << "--Iris :" ;

      msg << base_str.str() << level_str << std::string( out ) ;
      msg << "\n" ;
      if( log_data.mode != Log::Mode::Quiet )
      {
        if( log_data.use_stdout ) 
        {
          std::cout << level_str << msg.str().data() ;
        }
        
        cx = snprintf( log_data.log + log_data.current_pos, log_data.log_size, msg.str().data() ) ;
        
        if( cx < 0 || log_data.current_pos + cx > log_data.log_size )
        {
          log_data.write() ;
          std::memset( log_data.log, 0x0, 2 ) ;
        }
        else
        {
          log_data.current_pos += cx ;
        }
      }
      log_mutex.unlock() ;
    }
  }
}
