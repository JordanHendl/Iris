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

static athena::Manager manager     ;
static iris::Manager   mod_manager ;
static std::string     module_path ;
static std::string     config_path ;

bool testModManager()
{
  mod_manager.initialize( module_path.c_str(), config_path.c_str() ) ;
  return true ;
} 

int main( int argc, char* argv[] )
{
  std::string path = argv[ 0 ] ;
  argc = argc ;
  
  manager.initialize( "Iris Module Library" ) ;
  const size_t last_slash_idx = path.find_last_of( "\\/" ) ;
  if ( std::string::npos != last_slash_idx )
  {
    path.erase( last_slash_idx + 1, path.size() ) ;
  }
  
  std::cout << path << std::endl ;
  
  // This is because visual studio ( and others? ) build to a "Debug' or "Release" folder inside the binary build folder.
  #if( WIN32 )
    module_path = path + std::string( "../../test_module/") ; 
  #else 
    module_path = path + std::string( "../test_module/" ) ;
  #endif

  config_path = path + std::string( "test_config.json" ) ;
  
  std::cout << "\n-- Performing Iris Module Library Test. " << std::endl ;
  
  return manager.test( athena::Output::Verbose ) ; 
}