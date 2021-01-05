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
#include <KT/Manager.h>

/** Test JSON file.
 */
static const char* json_data = 

"{\n"
"  \"Graphs\":\n"
"  {\n"
"    \"graph_1\":\n"
"    {\n"
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
"      }\n"
"    }\n"
"  }\n"
"}\n\0\0" ;

static karma::config::json::Parser parser     ;
static karma::test::Manager        manager    ;
static karma::config::json::Token  base_token ;

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
    return false ; // CASE: Token lookup didn't find correct token.
  }
  
  for( auto tmp = token.begin(); tmp != token.end(); ++tmp )
  {
    name = tmp.key() ;
    
    if( name != "API" && name != "window" )
    {
      return false ; // CASE: If found something other than the correct modules.
    }
    
    iter += 1 ;
  }
  
  if( iter != 2 ) return false ; // CASE: It didnt iterate through both.
  return true ;
}

int main( int argc, const char** argv )
{
  parser.initialize( json_data ) ;
  
  base_token = parser.begin() ;
  manager.add( "Object Test"       , &testObject        ) ;
  manager.add( "Value Test"        , &testValue         ) ;
  manager.add( "Full Module Test"  , &testGettingValues ) ;
  manager.add( "Integer Value Test", &testIntegerValue  ) ;
  

  std::cout << "\nTesting Karma Configuration & JSON Parsing." << std::endl ;
  return manager.test( karma::test::Output::Verbose ) ;
}