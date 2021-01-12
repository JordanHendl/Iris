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

#include "Parser.h"
#include <log/Log.h>
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
      using Log = ::karma::log::Log ;

      struct JSONNode
      {
        typedef std::vector<std::string>        NodeList ;
        typedef std::map<std::string, JSONNode> NodeMap  ;
        friend class ParserData ;
        
      private:
        NodeMap  children ;
        NodeList values   ;
      public:

        JSONNode() = default ;
        
        const std::string& value( unsigned index ) const 
        {
          static std::string dummy = "" ;
          if( index < this->values.size() ) return this->values[ index ] ;
          
          return dummy ;
        }

        NodeMap::const_iterator find( std::string str ) const 
        {
          return this->children.find( str ) ;
        }
        
        NodeMap::const_iterator begin() const 
        {
          return this->children.begin() ;
        }
        
        NodeMap::const_iterator end() const 
        {
          return this->children.end() ;
        }
        
        JSONNode& operator[]( std::string str )
        {
          this->children.insert( { str, JSONNode() } ) ;

          return this->children[ str ] ;
        }
        
        unsigned size() const 
        {
          return this->values.size() ;
        }

        void clear()
        {
          this->values  .clear() ;
          this->children.clear() ;
        }

        bool leaf() const 
        {
          return this->children.empty() ;
        }
      };
      
      typedef JSONNode JSONMap ;

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
        
        /** Node.
         */
        const JSONNode* node ;
        
        /** Position in this object's node's values this token is at.
         */
        unsigned position ;

        /** The iterator of this token in the map.
         */
        JSONNode::NodeMap::const_iterator it  ; 
        
        TokenData()
        {
          this->node     = nullptr ;
          this->position = 0       ;
        }

        TokenData& operator=( const TokenData& data )
        {
          this->node     = data.node     ;
          this->position = data.position ;
          this->it       = data.it       ;
          
          return *this ;
        }

        /** Method to return this object's key.
         * @return const char* The C-String representation of this object's key.
         */
        const std::string& key() const
        {
          return this->it->first ;
        }

        /** Method to return the size of this object's value.
         * @return unsigned The size of this object's value array.
         */
        unsigned size() const
        {
          return this->node != nullptr ? this->node->size() : 0 ;
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
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleKey( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected number value input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleNumValue( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected string value input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleStringValue( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected boolean value input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleBoolValue( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected comma input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleComma  ( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected colon input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleColon  ( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected open-bracket input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleArray  ( JSONMap &parent, JSONFile& stream ) ;

        /** Handles an expected open-brace input in the stream.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleObject ( JSONMap &parent, JSONFile& stream ) ;

        /** Default catch-all handling. Searches file for next character and processes accordingly.
         * @param parent The concatenated string from this token's parent.
         * @param stream The stream to use for document data.
         */
        void handleNext   ( JSONMap &parent, JSONFile& stream ) ;
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
        bool in_comment ;
        char val        ;
        
        in_comment = false ;

        // While not eof, not whitespace, and not a line break.
        while( ( val = json_stream.get() ) != json_stream.eof() && ( val == ' ' || val == '\n' || val == '\r' || val == '#' || in_comment ) )
        { 
          if( val == '#' )
          {
            in_comment = true  ;
          }
          
          if( in_comment && ( val == '\n'|| val == '\r' ) )
          {
            in_comment = false ;
          }
        }

        return val ;
      }

      void ParserData::handleComma( JSONMap &parent, JSONFile& stream )
      {
        const char next = getNextValidCharacter ( stream ) ;  ///< TODO

        switch( next )
        {
          // EXPECTED: We found key, want delimiter
          case ':': /* Key:Value delimiter.*/ handleColon ( parent, stream ) ; break ;
          case '"': /* Start of an string. */ handleKey   ( parent, stream ) ; break ;
          case '{': /* Start of an object. */ handleObject( parent, stream ) ; break ;
          case '[': /* Start of an array.  */ handleArray ( parent, stream ) ; break ;

          // INVALID JSON 
          case ',': /* Continuing a list.  */ 
          case ' ': /* White space. Skip.  */ 
          case ']': /* End of an array.    */ 
          case '}': /* End of an object.   */ 
          default : /* Error               */ stream.putback( next ) ; Log::output( Log::Level::Warning, "Invalid JSON Found: ", next ) ; break ;
        }
      }

      void ParserData::handleColon( JSONMap &parent, JSONFile& stream )
      {
        const char next = getNextValidCharacter ( stream ) ; 
        
        
        if     ( isdigit( next ) || next  == '.' ) { stream.putback( next ) ; handleNumValue ( parent, stream ) ; }
        else if( next == 't' || next == 'f'      ) { stream.putback( next ) ; handleBoolValue( parent, stream ) ; }
        else
        {
          switch( next )
          {
            // EXPECTED: We found key, can be string, object, or array.
            case '"': /* Start of an value . */ handleStringValue( parent, stream ) ; break ;
            case '{': /* Start of an object. */ handleObject     ( parent, stream ) ; break ;
            case '[': /* Start of an array.  */ handleArray      ( parent, stream ) ; break ;
  
            // INVALID JSON 
            case ':': /* Key:Value delimiter.*/ 
            case ',': /* Continuing a list.  */ 
            case ' ': /* White space. Skip.  */ 
            case '}': /* End of an object.   */ 
            case ']': /* End of an array.    */ 
            default : /* Could be # or bool  */ stream.putback( next ) ; Log::output( Log::Level::Warning, "Invalid JSON Found: ", next, ".\n" ) ; break ;
          }
        }
      }

      void ParserData::handleArray( JSONMap &parent, JSONFile& stream )
      {
        char     next ; ///< 
        unsigned it   ; ///< 
        
        it = 0 ;
        while( ( next = getNextValidCharacter( stream ) ) != ']' && next != stream.eof() )
        {
          if     ( isdigit( next ) || next  == '.' ) { stream.putback( next ) ; handleNumValue ( parent, stream ) ; }
          else if( next == 't' || next == 'f'      ) { stream.putback( next ) ; handleBoolValue( parent, stream ) ; }
          else
          switch( next )
          {
            // EXPECTED: We found key, can be string, object, or array.
            case '"': /* Start of an string. */ it++ ; handleStringValue( parent, stream ) ; break ;
            case '{': /* Start of an object. */ it++ ; handleObject     ( parent, stream ) ; break ;
            case ',': /* Continuing a list.  */ ; break ;
  
            // INVALID JSON 
            case ':': /* Key:Value delimiter.*/
            case ' ': /* White space. Skip.  */
            case '}': /* End of an object.   */
            case ']': /* End of an array.    */ stream.putback( next ) ; Log::output( Log::Level::Warning, "Invalid JSON Found: ", next, ".\n" ) ; break ;
          }
        }
      }

      void ParserData::handleObject( JSONMap &parent,  JSONFile& stream )
      {
        char next ;

        while( ( next = getNextValidCharacter( stream ) ) != '}' && next != stream.eof() )
        {
          switch( next )
          {
            // EXPECTED: We found key, want delimiter
            case '"': /* Start of an string. */ handleKey   ( parent, stream ) ; break ;
            case ',': /* Continuing a list.  */ handleComma ( parent, stream ) ; break ;
  
            // INVALID JSON 
            case ':': /* Key:Value delimiter.*/ 
            case '{': /* Start of an object. */ 
            case '[': /* Start of an array.  */ 
            case ' ': /* White space. Skip.  */ 
            case '}': /* End of an object.   */ 
            case ']': /* End of an array.    */ 
            default : /* Error               */ stream.putback( next ) ; Log::output( Log::Level::Warning, "Invalid JSON Found: ", next, ".\n" ) ; break ;
          }
        }
      }

      void ParserData::handleKey( JSONMap &parent, JSONFile& stream )
      {
        const std::string str  = getString( stream )             ;
        const char        next = getNextValidCharacter( stream ) ;
        
        switch( next )
        {
          // EXPECTED: We found key, want delimiter
          case ':': /* Key:Value delimiter.*/ handleColon( parent[ str ], stream ) ; break ;

          // INVALID JSON 
          case ',': /* Continuing a list.  */ 
          case '"': /* Start of an string. */ 
          case '{': /* Start of an object. */ 
          case '[': /* Start of an array.  */ 
          case ' ': /* White space. Skip.  */ 
          case '}': /* End of an object.   */ 
          case ']': /* End of an array.    */ 
          default : /* Could be # or bool  */ stream.putback( next ) ; stream.putback( next ) ; Log::output( Log::Level::Warning, "Invalid JSON Found: ", next, ".\n" ) ; break ;
        }
      }

      void ParserData::handleNext( JSONMap &parent, JSONFile& stream )
      {
        const char value = getNextValidCharacter( stream ) ;
        
        switch( value )
        {
          case '"': /* Start of an string. */ handleKey   ( parent, stream ) ; break ;
          case '{': /* Start of an object. */ handleObject( parent, stream ) ; break ;
          case '[': /* Start of an array.  */ handleArray ( parent, stream ) ; break ;
          case ',': /* Continuing a list.  */ handleComma ( parent, stream ) ; break ;
          case ':': /* Key:Value delimiter.*/ handleColon ( parent, stream ) ; break ;
          case ' ': /* White space. Skip.  */ 
          case '}': /* End of an object.   */ 
          case ']': /* End of an array.    */ 
          default : /* Error               */ Log::output( Log::Level::Warning, "Invalid JSON Found: ", value, ".\n" ) ; break ;
        }
      }

      void ParserData::handleNumValue( JSONMap &parent, JSONFile& stream )
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
          parent.values.push_back( str.str() ) ;
        }
      }

      void ParserData::handleBoolValue( JSONMap &parent, JSONFile& stream )
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
          parent.values.push_back( buffer ) ;
        }
      }

      void ParserData::handleStringValue( JSONMap &parent, JSONFile& stream )
      {
        const std::string str  = getString( stream ) ;
        
        parent.values.push_back( str ) ;
      }

      void ParserData::processFile( JSONFile& stream )
      {
        while( !stream.eof() )
        {
          handleNext( this->map, stream ) ;          
        }
      }
  
      Parser::Parser() 
      {
        this->parser_data = new ParserData() ;
      }
  
      Parser::~Parser()
      {
        delete this->parser_data ;
      }

      Token Parser::find( const char* key ) const
      {
        Token token ;
        token.data().node = &data().map ;
        token.data().it = data().map.find( key ) ;
        return token ;
      }

      Token Parser::end() const
      {
        Token token ;
        token.data().node = &data().map ;
        token.data().it   = data().map.end() ;
        return token ;
      }

      Token Parser::begin() const
      {
        Token token ;
        token.data().node = &data().map        ;
        token.data().it   = data().map.begin() ;
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
        
      Token::operator bool() const
      {
        if( data().node == nullptr ) return false ;
        return true ;
      }

      Token Token::operator[]( const char* key ) const
      {
        Token tmp ;
        Token ret ;
        
        ret = *this ;
        
        // Base case.
        if( data().node == nullptr || data().it == data().node->end() || data().key() == key )
        {
          ret = *this ;
          
          return ret ;
        }
        
        // Look through this node's immidiate children.

        // We didnt find it immidiately, so now we have to recursively look.
        for( auto json = data().node->begin(); json != data().node->end(); ++json )
        {
          // If this map location is the correct one, return it.
          if( json->first == key )
          {
            ret             = *this  ;
            ret.data().it   = json   ;
            
            return ret ;
          }
          
          if( !json->second.leaf() )
          {
            // Else, set temporary token to the child & look up.
            tmp.data().node     = &json->second            ;
            tmp.data().it       = tmp.data().node->begin() ;
            tmp.data().position = 0                        ;
  
            // Recursively search child for key.
            ret = tmp[ key ] ;
              
            // If we found it, return.
            if( ret.data().node != nullptr && ret.data().it != ret.data().node->end() && std::string( ret.key() ) == key )
            {
              return ret ;
            }
          }
        }
        
        // Found nothing, return this.
        return Token() ;
      }
      
      void Token::operator=( const Token& token )
      {
        *this->token_data = *token.token_data ;
      }

      bool Token::operator!=( const Token& token )
      {
        return data().it != token.data().it ;
      }
      
      Token Token::begin() const
      {
        Token tmp ;
        
        if( data().node != nullptr )
        {
          tmp             = *this                    ;
          tmp.data().node = &data().it->second       ;
          tmp.data().it   = tmp.data().node->begin() ;
        }
        
        return tmp ;
      }
      
      Token Token::end() const
      {
        Token tmp ;
        
        if( data().node != nullptr )
        {
          tmp             = *this                  ;
          tmp.data().node = &data().it->second     ;
          tmp.data().it   = tmp.data().node->end() ;
        }
        
        return tmp ;
      }

      const char* Token::key() const
      {
        return data().key().c_str() ;
      }

      const char* Token::string( unsigned index ) const
      {
        if( data().node != nullptr )
        {
          return data().it->second.value( index ).c_str() ;
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
        if( data().node != nullptr )
        {
          return static_cast<float>( std::atof(data().it->second.value( index ).c_str() ) ) ;
        }
        else
        {
          return 0.0f ;
        }
      }

      bool Token::boolean( unsigned index ) const
      {
        if( data().node != nullptr )
        {
          if      ( data().it->second.value( index ) == "false" ) return false ;
          else if ( data().it->second.value( index ) == "true"  ) return true  ;
        }
        
        return false ;
      }

      unsigned Token::number( unsigned index ) const
      {
        if( data().node != nullptr && data().it != data().node->end() )
        {
          return static_cast<unsigned>( std::atoi( data().it->second.value( index ).c_str() ) ) ;
        }
        else
        {
          return 0 ;
        }
      }

      bool Token::isArray() const
      {
        return data().node != nullptr ? data().node->size() > 1 : false ;
      }

      void Token::operator++()
      {
        if( data().node != nullptr )
        {
          if( data().it != data().node->end() )
          {
            ++data().it ;
          }
        }
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
