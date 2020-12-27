/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Parser.cpp
 * Version      : 1.0
 * Date created : 6/21/2020
 * Purpose      : Definitions for JSON Token & Parser classes for retrieving data from a JSON input.
**********************************************************************/
#include "Parser.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <variant>
#include <map>

namespace karma
{
  namespace config
  {
    namespace json
    {
      typedef std::vector<std::string>            TokenValues ;
      typedef  std::map<std::string, TokenValues> JSONMap     ;

      /** Function to recieve the next valid JSON character from the stream.
       * @param json_stream Reference to the stringstream to search through.
       * @return char The character that is valid.
       */
      static char getNextValidCharacter( std::stringstream& json_stream ) ;
      
      /** Helper function to retrieve the rest of a string when a string is seen in the JSON file
       * @param json_stream Reference to the stringstream to search through.
       * @return std::string The string value without the double quotes.
       */
      static std::string getString( std::stringstream& json_stream ) ;

      /** Structure to contain a token's internal data.
       */
      struct TokenData
      {
        /** The iterator of this token in the map.
         */
        JSONMap::const_iterator it  ; 

        /** Method to return this object's key.
         * @return const char* The C-String representation of this object's key.
         */
        const char* key() const
        {
          return it->first.c_str() ;
        }

        /** Method to return the size of this object's value.
         * @return unsigned The size of this object's value array.
         */
        unsigned size() const
        {
          return it->second.size() ;
        }
      };
      
      /** Structure to contain the parser's internal data.
       */
      struct ParserData
      {
        typedef std::stringstream& JSONFile ;

        /** The mapping of the json file's keys to values.
         */
        JSONMap map ; 

        /** Root handling of the JSON file.
         * @param stream The stream to use for document data.
         */
        void processFile   ( JSONFile& stream ) ;

        /** Handles an expected Key input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleKey( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected number value input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleNumValue( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected string value input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleStringValue( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected boolean value input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleBoolValue( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected comma input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleComma  ( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected colon input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleColon  ( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected open-bracket input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleArray  ( std::string parent_key, JSONFile& stream ) ;

        /** Handles an expected open-brace input in the stream.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleObject ( std::string parent_key, JSONFile& stream ) ;

        /** Default catch-all handling. Searches file for next character and processes accordingly.
         * @param parent_key The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleNext   ( std::string parent_key, JSONFile& stream ) ;
      };

      std::string getString( std::stringstream& json_stream )
      {
        std::stringstream stream ;
        char              val    ;

        while( ( val = json_stream.get() ) != json_stream.eof() && val != '"' )
        {
          stream << val ;
        }

        return stream.str() ;
      }

      char getNextValidCharacter( std::stringstream& json_stream )
      {
        char val ;

        // While not eof, not whitespace, and not a line break.
        while( ( val = json_stream.get() ) != json_stream.eof() && ( val == ' ' || val == '\n' || val == '\r' ) ) { }

        return val ;
      }

      void ParserData::handleComma( std::string parent_key, JSONFile& stream )
      {
        const char next = getNextValidCharacter ( stream ) ;  ///< TODO

        switch( next )
        {
          // EXPECTED: We found key, want delimiter
          case ':': /* Key:Value delimiter.*/ handleColon ( parent_key, stream ) ; break ;
          case '"': /* Start of an string. */ handleKey   ( parent_key, stream ) ; break ;
          case '{': /* Start of an object. */ handleObject( parent_key, stream ) ; break ;
          case '[': /* Start of an array.  */ handleArray ( parent_key, stream ) ; break ;

          // INVALID JSON 
          case ',': /* Continuing a list.  */ stream.putback( next ) ; break ;
          case ' ': /* White space. Skip.  */ stream.putback( next ) ; break ;
          case ']': /* End of an array.    */ stream.putback( next ) ; break ;
          case '}': /* End of an object.   */ stream.putback( next ) ; break ;
          default : /* Error               */ stream.putback( next ) ; break ;
        }
      }

      void ParserData::handleColon( std::string parent_key, JSONFile& stream )
      {
        const char next = getNextValidCharacter ( stream ) ; 
        
        
        if     ( isdigit( next ) || next  == '.' ) { stream.putback( next ) ; handleNumValue ( parent_key, stream ) ; }
        else if( next == 't' || next == 'f'      ) { stream.putback( next ) ; handleBoolValue( parent_key, stream ) ; }
        else
        {
          switch( next )
          {
            // EXPECTED: We found key, can be string, object, or array.
            case '"': /* Start of an value . */ handleStringValue( parent_key, stream ) ; break ;
            case '{': /* Start of an object. */ handleObject     ( parent_key, stream ) ; break ;
            case '[': /* Start of an array.  */ handleArray      ( parent_key, stream ) ; break ;
  
            // INVALID JSON 
            case ':': /* Key:Value delimiter.*/ stream.putback( next ) ; break ;
            case ',': /* Continuing a list.  */ stream.putback( next ) ; break ;
            case ' ': /* White space. Skip.  */ stream.putback( next ) ; break ;
            case '}': /* End of an object.   */ stream.putback( next ) ; break ;
            case ']': /* End of an array.    */ stream.putback( next ) ; break ;
            default : /* Could be # or bool  */ stream.putback( next ) ; break ;
          }
        }
      }

      void ParserData::handleArray( std::string parent_key, JSONFile& stream )
      {
        char     next ; ///< 
        unsigned it   ; ///< 
        
        it = 0 ;
        while( ( next = getNextValidCharacter( stream ) ) != ']' && next != stream.eof() )
        {
          if     ( isdigit( next ) || next  == '.' ) { stream.putback( next ) ; handleNumValue ( parent_key, stream ) ; }
          else if( next == 't' || next == 'f'      ) { stream.putback( next ) ; handleBoolValue( parent_key, stream ) ; }
          else
          switch( next )
          {
            // EXPECTED: We found key, can be string, object, or array.
            case '"': /* Start of an string. */ it++ ; handleStringValue( parent_key, stream ) ; break ;
            case '{': /* Start of an object. */ it++ ; handleObject     ( parent_key, stream ) ; break ;
            case ',': /* Continuing a list.  */ ; break ;
  
            // INVALID JSON 
            case ':': /* Key:Value delimiter.*/ break ;
            case ' ': /* White space. Skip.  */ break ;
            case '}': /* End of an object.   */ break ;
            case ']': /* End of an array.    */ break ;
          }
        }
      }

      void ParserData::handleObject( std::string parent_key,  JSONFile& stream )
      {
        char  next ;

        while( ( next = getNextValidCharacter( stream ) ) != '}' && next != stream.eof() )
        {
          switch( next )
          {
            // EXPECTED: We found key, want delimiter
            case '"': /* Start of an string. */ handleKey   ( parent_key, stream ) ; break ;
            case ',': /* Continuing a list.  */ handleComma ( parent_key, stream ) ; break ;
  
            // INVALID JSON 
            case ':': /* Key:Value delimiter.*/ break ;
            case '{': /* Start of an object. */ break ;
            case '[': /* Start of an array.  */ break ;
            case ' ': /* White space. Skip.  */ break ;
            case '}': /* End of an object.   */ break ;
            case ']': /* End of an array.    */ break ;
            default : /* Error               */ break ;
          }
        }
      }

      void ParserData::handleKey( std::string parent_key, JSONFile& stream )
      {
        const std::string str  = getString( stream )                                    ;
        const std::string key  = parent_key.size() != 0 ? parent_key + "::" + str : str ;
        const char        next = getNextValidCharacter( stream )                        ;
        
//        if( parent_key != "" && this->map.find( parent_key ) == this->map.end() )
        {
          this->map[ parent_key ].push_back( str ) ;
        }

        switch( next )
        {
          // EXPECTED: We found key, want delimiter
          case ':': /* Key:Value delimiter.*/ handleColon  ( key, stream ) ; break ;

          // INVALID JSON 
          case ',': /* Continuing a list.  */ stream.putback( next ) ; break ;
          case '"': /* Start of an string. */ stream.putback( next ) ; break ;
          case '{': /* Start of an object. */ stream.putback( next ) ; break ;
          case '[': /* Start of an array.  */ stream.putback( next ) ; break ;
          case ' ': /* White space. Skip.  */ stream.putback( next ) ; break ;
          case '}': /* End of an object.   */ stream.putback( next ) ; break ;
          case ']': /* End of an array.    */ stream.putback( next ) ; break ;
          default : /* Could be # or bool  */ stream.putback( next ) ; break ;
        }
      }

      void ParserData::handleNext( std::string parent_key, JSONFile& stream )
      {
        const char value = getNextValidCharacter( stream ) ;

        switch( value )
        {
          case '"': /* Start of an string. */ handleKey   ( parent_key, stream ) ; break ;
          case '{': /* Start of an object. */ handleObject( parent_key, stream ) ; break ;
          case '[': /* Start of an array.  */ handleArray ( parent_key, stream ) ; break ;
          case ',': /* Continuing a list.  */ handleComma ( parent_key, stream ) ; break ;
          case ':': /* Key:Value delimiter.*/ handleColon ( parent_key, stream ) ; break ;
          case ' ': /* White space. Skip.  */ break ;
          case '}': /* End of an object.   */ break ;
          case ']': /* End of an array.    */ break ;
          default : /* Error               */ break ;
        }
      }

      void ParserData::handleNumValue( std::string parent_key, JSONFile& stream )
      {
        std::stringstream str  ;
        char              next ;

        while( isdigit( next = getNextValidCharacter( stream ) ) || next == '.' )
        {
          str << next ;
        }

        stream.putback( next ) ;

        if( str.str() != "" )
        {
          this->map[ parent_key ].push_back( str.str() ) ;
        }
      }

      void ParserData::handleBoolValue( std::string parent_key, JSONFile& stream )
      {
        const char next = getNextValidCharacter( stream ) ;
        std::string       buffer ;

        if( next == 't')
        {
          buffer.resize( 4 ) ;
          stream.putback( next ) ;
          stream.read( &buffer[0], 4 ) ;

          if( buffer != "true" ) for( unsigned i = 0; i < 4; i++ ) stream.putback( buffer[i] ) ;
        }
        else if( next == 'f')
        {
          buffer.resize( 5 ) ;
          stream.putback( next ) ;
          stream.read( &buffer[0], 5 ) ;

          if( buffer != "false" ) for( unsigned i = 0; i < 5; i++ ) stream.putback( buffer[i] ) ;
        }

        if( buffer.size() != 0 )
        {
          this->map[ parent_key ].push_back( buffer ) ;
        }
      }

      void ParserData::handleStringValue( std::string parent_key, JSONFile& stream )
      {
        const std::string str  = getString( stream ) ;

        this->map[ parent_key ].push_back( str ) ;
      }

      void ParserData::processFile( JSONFile& stream )
      {
        while( !stream.eof() )
        {
          handleNext( "", stream ) ;          
        }
      }
  
      Parser::Parser() 
      {
        this->parser_data = new ParserData() ;
      };
  
      Parser::~Parser()
      {
        delete this->parser_data ;
      }

      Token Parser::find( const char* key ) const
      {
        Token token ;
        token.data().it = data().map.find( key ) ;
        return token ;
      }

      Token Parser::end() const
      {
        Token token ;
        token.data().it = data().map.end() ;
        return token ;
      }

      Token Parser::begin() const
      {
        Token token ;
        token.data().it = data().map.begin() ;
        return token ;
      }

      void Parser::initialize( const char* json )
      {
        std::stringstream     stream ;
        std::string::iterator it     ;
        std::string           stage  ;

        stream << json ;
        data().processFile( stream ) ;
      }
      
      void Parser::clear()
      {
        data().map.clear() ;
      }

      ParserData& Parser::data()
      {
        return *this->parser_data ;
      }

      const ParserData& Parser::data() const 
      {
        return *this->parser_data ;
      }

      Token::Token()
      {
        this->token_data = new TokenData() ;
      }

      Token::~Token()
      {
        delete this->token_data ;
      }

      Token::Token( const Token& token ) 
      {
        this->token_data = new TokenData() ;

        *this->token_data = *token.token_data ;
      }

      void Token::operator=( const Token& token )
      {
        *this->token_data = *token.token_data ;
      }

      bool Token::operator!=( const Token& token )
      {
        return data().it != token.data().it ;
      }

      const char* Token::key() const
      {
        return data().key() ;
      }

      const char* Token::string( unsigned index ) const
      {
        if( index < data().it->second.size() )
        {
          return data().it->second[ index ].c_str() ;
        }
        else
        {
          return "" ;
        }
      }

      unsigned Token::size() const
      {
        return data().size() ;
      }

      float Token::decimal( unsigned index ) const
      {
        if( index < data().it->second.size() )
        {
          return static_cast<float>( std::atof( data().it->second[ index ].c_str() ) ) ;
        }
        else
        {
          return 0.0f ;
        }
      }

      bool Token::boolean( unsigned index ) const
      {
        if      ( data().it->second[ index ] == "false" ) return false ;
        else if ( data().it->second[ index ] == "true"  ) return true  ;
        
        return false ;
      }

      unsigned Token::number( unsigned index ) const
      {
        if( index < data().it->second.size() )
        {
          return static_cast<unsigned>( std::atoi( data().it->second[ index ].c_str() ) ) ;
        }
        else
        {
          return 0 ;
        }
      }

      bool Token::isArray() const
      {
        return data().it->second.size() > 1 ;
      }

      void Token::operator++()
      {
        ++data().it ;
      }

      TokenData& Token::data()
      {
        return *this->token_data ;
      }

      const TokenData& Token::data() const
      {
        return *this->token_data ;
      }
    }
  }
}
