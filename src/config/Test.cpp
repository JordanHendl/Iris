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

#include "Configuration.h"
#include "Parser.h"
#include <iostream>
#include <Athena/Manager.h>
#include <string>

/** Test JSON file.
 */
static const char* json_data = 

"{\n"
"  \"Graphs\":\n"
"  {\n"
"    \"graph_1\":\n"
"    {\n"
"      #This is a comment!\n"
"      \"Modules\" :\n"
"      {\n"
"        \"API\" :\n"
"        {\n"
"          \"type\" : \"Vulkan\"\n"
"        },\n"
"        \"window\" :\n"
"        {\n"
"          \"type\"   : \"Vulkan Window\",\n"
"          \"title\"  : \"Chauffeur\",\n"
"          \"width\"  : 1240,\n"
"          \"height\" : 1024\n"
"        }\n"
"        \"things\" :\n"
"        [\n"
"          {\n"
"             \"x\" : 0.2,\n"
"             \"y\" : 0.1,\n"
"             \"z\" : 0.5,\n"
"          },\n"
"          {\n"
"             \"x\" : 0.9,\n"
"             \"y\" : 0.8,\n"
"             \"z\" : 0.7,\n"
"          }\n"
"        ]\n"
"      }\n"
"    }\n"
"  }\n"
"}\n\0\0" ;


static athena::Manager            manager    ;
static iris::config::json::Parser parser     ;
static iris::config::json::Token  base_token ;

bool testGraphLookup()
{
  bool found_it = true ;
  auto token = base_token ;
  
//  for( auto& tok : token )
  {
//    if( std::string( tok.key() ) == "Graphs" ) found_it = true ;
  }
  
  if( !token[ "Graphs" ]                                 ) return false ;
  if( std::string( token[ "Graphs" ].key() ) != "Graphs" ) return false ;
  return found_it ;
}

bool testBadLookup()
{
  auto token = base_token[ "BadLookup" ] ;
  
  if( token ) return false ; // CASE: Lookup failed and said something invalid was valid.
  
  token = base_token[ "Modules" ] ;
  if( token ) return true ;
          
  return false ; // CASE: Lookup failed to recognize a module as valid.
}

bool testIntegerValue()
{
  auto token = base_token[ "Modules" ][ "window" ][ "width" ] ;
  std::string key = token.key() ;
  
  if( key            != "width" ) return false ; // CASE: Bad token lookup.
  if( token.number() != 1240    ) return false ; // CASE: Bad value lookup / conversion.
  return true ;
}

bool testGettingValues()
{
  auto token = base_token[ "Modules" ] ;
  std::string key   ;
  std::string val   ;
  int         found ;
  
  found = 0 ;
  for( auto module = token.begin(); module != token.end(); ++module )
  {
    key = module.key() ;
    
    if( key == "API" || key == "window" )
    {
      found++ ;
    }

    for( auto value = module.begin(); value != module.end(); ++value )
    {
      key = value.key()    ;
      val = value.string() ;
      
      if( key == "type" && val == "Vulkan" )
      {
        found++ ;
      }
      if( key == "type" && val == "Vulkan Window" )
      {
        found++ ;
      }
      if( key == "title" && val == "Chauffeur" )
      {
        found++ ;
      }
      if( key == "width" && val == "1240" )
      {
        found++ ;
      }
      if( key == "height" && val == "1024" )
      {
        found++ ;
      }
    }
  }
  
  if( found == 7 ) return true ; // CASE: It did not find all the required information.
  return false ;
}

bool testValue()
{
  auto token = base_token[ "Modules" ][ "API" ] ;
  std::string name = token.key() ; 
  unsigned iter = 0 ;
  if( name != "API" )
  {
    return false ; // CASE: Token lookup failed.
  }
  
  for( auto tmp = token.begin(); tmp != token.end(); ++tmp )
  {
    std::string key = tmp.key()    ;
    std::string str = tmp.string() ;

    if( key != "type" || str != "Vulkan" )
    {
      return false ; // CASE: It found something other than the correct key & value.
    }
    
    iter++ ;
  }
  
  if( iter != 1 ) return false ; // CASE: It's finding more or less values than expected.
  return true ;
}


bool testObject()
{
  auto token = base_token[ "Modules" ] ;
  unsigned iter = 0 ;
  std::string name = token.key() ;
  
  if( name != "Modules" )
  {
    std::cout << "Token lookup didn't find correct token. Expected: Modules. Got: " << name << std::endl ;
    return false ; // CASE: Token lookup didn't find correct token.
  }
  
  for( auto tmp = token.begin(); tmp != token.end(); ++tmp )
  {
    name = tmp.key() ;
    
    if( name != "API" && name != "window" && name != "things" )
    {
      std::cout << "Token lookup found something other than the correct value." << name << std::endl ;
      return false ; // CASE: If found something other than the correct modules.
    }
    
    iter += 1 ;
  }
  
  if( iter != 3 )
  {
    std::cout << "Token lookup didnt iterate through all parameters of the object." << std::endl ;
    return false ; // CASE: It didnt iterate through both.
  }
    
  return true ;
}

athena::Result testArrayObject()
{
  const auto token = base_token[ "Modules" ][ "things" ] ;
  std::string name ;
  
  name = token.key() ;
  
  if( name != "things"                           ) { std::cout << "Wrong name. Name expected: things. Name found: "                   << name                              << std::endl ; return false ; }
  if( token.size() != 2                          ) { std::cout << "Wrong size. Size expected: 2.      Size found: "                   << token.size()                      << std::endl ; return false ; }
  if( !token.token( 0 )[ "x" ]                   ) { std::cout << "Invalid token found when a valid one was expected. "               << token.size()                      << std::endl ; return false ; }
  if(  token.token( 0 )[ "x" ].decimal() != 0.2f ) { std::cout << "Array object token has bad value. Expected value: 0.2. Recieved: " << token.token( 0 )[ "x" ].decimal() << std::endl ; return false ; }
  if(  token.token( 1 )[ "x" ].decimal() != 0.9f ) { std::cout << "Array object token has bad value. Expected value: 0.9. Recieved: " << token.token( 1 )[ "x" ].decimal() << std::endl ; return false ; }
  if(  token.token( 0 )[ "y" ].decimal() != 0.1f ) { std::cout << "Array object token has bad value. Expected value: 0.1. Recieved: " << token.token( 0 )[ "y" ].decimal() << std::endl ; return false ; }
  if(  token.token( 1 )[ "y" ].decimal() != 0.8f ) { std::cout << "Array object token has bad value. Expected value: 0.8. Recieved: " << token.token( 1 )[ "x" ].decimal() << std::endl ; return false ; }
  return true ;
}

int main()
{
  parser.initialize( json_data ) ;
  
  base_token = parser.begin() ;

  manager.initialize( "Iris Configuration Tests" ) ;
  manager.add( "Object Test"       , &testObject        ) ;
  manager.add( "Value Test"        , &testValue         ) ;
  manager.add( "Full Module Test"  , &testGettingValues ) ;
  manager.add( "Integer Value Test", &testIntegerValue  ) ;
  manager.add( "Bad Lookup Test"   , &testBadLookup     ) ;
  manager.add( "Graph Lookup Test" , &testGraphLookup   ) ;
  manager.add( "Object Array Test" , &testArrayObject   ) ;
  

  return manager.test( athena::Output::Verbose ) ;
}