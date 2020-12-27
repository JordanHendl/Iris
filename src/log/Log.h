/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Log.h
 * Author: jhendl
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
    
    /**
     */
    class Log
    {
      public:

        /**
         */
        enum class Level
        {
          None,
          Warning,
          Fatal,
        };
        
        /** Static method to initialize the logging subsystem.
         * @param logging_directory The folder to store logs in.
         * @param use_stdout Whether or not logging should be dumped to stdout.
         */
        static void initialize( const char* logging_directory, bool use_stdout = true ) ;
        
        /**
         * @param first
         * @param args
         */
        static void write() ;

        /**
         * @param level
         * @param first
         * @param args
         */
        template<typename T, typename... ARGS>
        static void output( Level level, T first, ARGS... args ) ;

        /**
         * @param level
         * @param first
         * @param args
         */
        template<typename T>
        static void output( Level level, T first ) ;
        
        /**
         * @param first
         * @param args
         */
        template<typename T, typename... ARGS>
        static void output( T first, ARGS... args ) ;

        /**
         * @param first
         * @param args
         */
        template<typename T>
        static void output( T first ) ;
        
      private:
        
        /**
         * @param out
         * @param level
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

