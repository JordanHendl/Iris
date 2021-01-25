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

#include "Manager.h"
#include "Graph.h"
#include <Athena/Manager.h>
#include <iostream>
#include <ostream>
#include <fstream>

static athena::Manager manager      ;
static iris::Manager   mod_manager  ;
static std::string     module_path  ;
static std::string     config_path  ;
static std::ofstream   stream       ;

/** Test JSON file.
 */
static std::string json_data = 

"{\n"
"  \"graph_1\":\n"
"  {\n"
"    \"Modules\" :\n"
"    {\n"
"      \"Test\" :\n"
"      {\n"
"        \"type\"    : \"TestModule\",\n"
"        \"version\" :  1,\n"
"        \"thing1\"  :  0.25,\n"
"        \"thing2\"  :  \"thing2\",\n"
"        \"thing3\"  :  2503\n"
"      }\n"
"    }\n"
"  }\n"
"}\n\0\0" ;

bool testModManager()
{
  mod_manager.initialize( module_path.c_str(), config_path.c_str() ) ;
  mod_manager.start() ;
  return true ;
} 

int main( int argc, char* argv[] )
{
  std::string path = argv[ 0 ] ;
  argc = argc ;
  path = path.substr( 0, path.size() - 21 ) ;
  
  // This is because visual studio ( and others? ) build to a "Debug' or "Release" folder inside the binary build folder.
  #if( WIN32 )
    module_path = path + std::string( "../../test_module/") ; 
  #else 
    module_path = path + std::string( "/test_module/" ) ;
  #endif

  config_path = path + std::string( "test_config.json" ) ;
  stream.open( config_path ) ;
  if( stream )
  {
    stream.write( json_data.c_str(), json_data.size() ) ;
    stream.close() ;
  }
  
  std::cout << "\n Performing Iris Module Library Test. " << std::endl ;
  
  manager.add( "Module Manager & Graph Test", &testModManager ) ;

  return manager.test( athena::Output::Verbose ) ; 
}