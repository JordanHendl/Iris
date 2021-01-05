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
/* 
 * File:   Log.h
 * Author: Jordan Hendl
 *
 * Created on August 9, 2020, 5:06 AM
 */

#ifndef LOG_H
#define LOG_H

namespace karma
{
  namespace log
  {
    class String ;
    
    /**
     * @param first
     * @param second
     */
    void operator<<( String& first, char second ) ;
    
    /**
     * @param first
     * @param second
     */
    void operator<<( String& first, const char* second ) ;
    
    /**
     * @param first
     * @param second
     */
    void operator<<( String& first, unsigned second ) ;
    
    /**
     * @param first
     * @param second
     */
    void operator<<( String& first, double second ) ;

    /**
     * @param first
     * @param second
     */
    void operator<<( String& first, float second ) ;
    
    /**
     * @param first
     * @param second
     */
    void operator<<( String& first, const String& second ) ;

    /**
     * @param type
     * @return 
     */
    template<typename T>
    String concatenate( T type ) ;
    
    /**
     * @param first
     * @param types
     * @return 
     */
    template<typename T, typename... TYPES>
    String concatenate( T first, TYPES... types ) ;

    /**
     */
    class String
    {
      public:
        
        /**
         * @param string
         */
        String() ;
        
        /**
         * @param string
         */
        String( const String& string ) ;
        
        /**
         * @return 
         */
        void operator=( const String& string ) ;
        
        /**
         * @param str
         */
        ~String() ;
        
        /**
         * @param str
         */
        void setStr( const char* str ) ;
        
        /**
         * @return 
         */
        const char* str() const ;
        
      private:
        
        /**
         */
        struct StringData* string_data ;
        
        /**
         * @return 
         */
        StringData& data() ;
        
        /**
         * @return 
         */
        const StringData& data() const ;
    };
    
    /** Class to handle logging in the Karma library.
     */
    class Log
    {
      public:

        /** Log levels, describing the severity of the log entry.
         */
        enum class Level
        {
          None,
          Warning,
          Fatal,
        };
        
        enum class Mode
        {
          Quiet,
          Normal,
          Verbose
        };
        
        /** Static method to initialize the logging subsystem.
         * @param logging_directory The folder to store logs in.
         * @param use_stdout Whether or not logging should be dumped to stdout.
         */
        static void initialize( const char* logging_directory, bool use_stdout = true ) ;
        
        /** Static method to flush the log to disk.
         */
        static void flush() ;
        
        /** Method to set the log mode.
         * @param mode The mode to use for log output.
         */
        static void setMode( Mode mode ) ;

        /** Variadic template function for outputting logs.
         * @param level The log level.
         * @param first The first paremeter
         * @param args Any other logging information.
         */
        template<typename T, typename... ARGS>
        static void output( Level level, T first, ARGS... args ) ;

        /** Variadic template method for outputting logs.
         * @param level The log level.
         * @param first The first entry.
         */
        template<typename T>
        static void output( Level level, T first ) ;
        
        /** Variadic template method for outputting logs.
         * @param first The first entry
         * @param args Any additional arguments.
         */
        template<typename T, typename... ARGS>
        static void output( T first, ARGS... args ) ;

        /** Base-case for variadic template methods.
         * @param first The log information to output.
         */
        template<typename T>
        static void output( T first ) ;
        
      private:
        
        /** Base-case for variadic template.
         * @param out The completed log string.
         * @param level The log level.
         */
        static void outputBase( const char* out, Level level ) ;
    };
    
    template<typename T>
    String concatenate( T type )
    {
      String str ;
      
      str << type ;
      return str ;
    }

    template<typename T, typename... TYPES>
    String concatenate( T first, TYPES... types )
    {
      String str ;
      
      str << first ;
      str << concatenate( types... ) ;
      return str ;
    }

    template<typename T, typename... ARGS>
    void Log::output( T first, ARGS... args )
    {
      Log::Level level ;
      String     str   ;
      
      str   = ::karma::log::concatenate( first, args... ) ;
      level = Log::Level::None                            ;

      outputBase( str.str(), level ) ;
    }

    template<typename T>
    void Log::output( T first )
    {
      Log::Level level ;
      String     str   ;
      
      str << first ;
      level = Log::Level::None ;

      outputBase( str.str(), level ) ;
    }

    template<typename T, typename... ARGS>
    void Log::output( Log::Level level, T first, ARGS... args )
    {
      String str ;
      
      str = ::karma::log::concatenate( first, args... ) ;

      outputBase( str.str(), level ) ;
    }

    template<typename T>
    void Log::output( Log::Level level, T first )
    {
      String str ;
      
      str << first ;

      outputBase( str.str(), level ) ;
    }
  }
}

#endif /* LOG_H */

