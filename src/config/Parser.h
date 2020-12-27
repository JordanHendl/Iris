/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Parser.h
 * Version      : 1.0
 * Date created : 6/21/2020
 * Purpose      : Declaration for JSON Token & Parser classes for retrieving data from a JSON input.
**********************************************************************/
#ifndef HZ_CONFIG_PARSER_H
#define HZ_CONFIG_PARSER_H

namespace karma
{
  namespace config
  {
    namespace json
    {
      /** Class to act as an iterator for the parser's internal map.
       */
      class Token
      {
        public:

          /** Default Constructure. Allocates this object's data.
           */
          Token() ;

          /** Default deconstructure. Releases this object's allocated data.
           */
          ~Token() ;

          /** Copy contructor. Copies the objects data from another copy.
           * @param token Const-reference to the object to copy data from.
           */
          Token( const Token& token ) ;

          /** Assignment operator for another copy of this object.
           * @param token Const-reference to the other object to copy data from.
           */
          void operator=( const Token& token ) ;

          /** Inequality operator for another copy of this object.
           * @param token The token to compare iterators to.
           * @return Whether or not this object is equivalent to the provided one.
           */
          bool operator!=( const Token& token ) ;

          /** Method to return whether or not this token's value is a part of an array. Used to iterate over array for values.
           * @return Whether or not this object represents a value inside an array.
           */
          bool isArray() const ;

          /** Method to recieve the key that is associated with this Token's key:value pair.
           * @return The key of this token's key:value pair.
           */ 
          const char* key() const ;

          /** Method to recieve the value of this token as a C-style string.
           * @param index The index of the object, if it is part of an array, to recieve.
           * @return The C-String representation of the value of the token at the specified index.
           */
          const char* string( unsigned index = 0 ) const ;

          /** Method to recieve the value of this token as a C-style string.
           * @param index The index of the object, if it is part of an array, to recieve.
           * @return The C-String representation of the value of the token at the specified index.
           */
          unsigned number( unsigned index = 0 ) const ;

          /** Method to recieve the value of this token as a C-style string.
           * @param index The index of the object, if it is part of an array, to recieve.
           * @return The C-String representation of the value of the token at the specified index.
           */
          float decimal( unsigned index = 0 ) const ;

          /** Method to recieve the value of this token as a C-style string.
           * @param index The index of the object, if it is part of an array, to recieve.
           * @return The C-String representation of the value of the token at the specified index.
           */
          bool boolean( unsigned index = 0 ) const ;

          /** Method to return the size of this object's data array.
           * @note If this object is not an array, this is always 1.
           * @return The size of this object's array.
           */
          unsigned size() const ;

          /** Pre-increment operator. Used for iterating over this object's map.
           */
          void operator++() ;

          /** Friend decleration of Parser class.
           */
          friend class Parser ;

        private:
        
          /** Forward-declared container for this object's data.
           */
          struct TokenData *token_data ;

          /** Method to retrieve a reference to this object's internal data.
           * @return A reference to this object's internal data.
           */
          TokenData& data() ;

          /** Method to retrieve a const-reference to this object's internal data.
           * @return A const-reference to this object's internal data.
           */
          const TokenData& data() const ;
      };

      /** Parser class for parsing JSON input.
       */
      class Parser
      {
        public:

          /** Default constructor. Allocates this object's data.
           */ 
          Parser() ;

          /** Default deconstructor. Releases this object's allocated data.
           */ 
          ~Parser() ;

          /** Initializes this object with the given input JSON text.
           * @note input is NOT a path.
           * @param input JSON Text to be parsed.
           */
          void initialize( const char* input ) ;

          /** Method to retrieve a token at a specific key, if it exists. Otherwise returns a token at the end of the map.
           * @param key The key to search for in the map.
           * @return Token The token places at the key:value found in the mapping.
           */
          Token find( const char* key ) const ;
          
          /** Method to clear this object's internal data.
           */
          void clear() ;

          /** Returns a Token pointing to the beginning of this object's internal mapping.
           * @return A Token pointing at the beginning of this object's internal mapping.
           */
          Token begin() const ;

          /** Returns a Token pointing to the end of this object's internal mapping.
           * @return A Token pointing at the end of this object's internal mapping.
           */
          Token end() const ;

        private:

          /** Forward declared structure to contain this object's interal data.
           */
          struct ParserData *parser_data ;

          /** Method to retrieve a reference to this object's internal data.
           * @return A reference to this object's internal data.
           */
          ParserData& data() ;

          /** Method to retrieve a const-reference to this object's internal data.
           * @return A const-reference to this object's internal data.
           */
          const ParserData& data() const ;
      };
    }
  }
}
#endif