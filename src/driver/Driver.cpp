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
 * along with this program.  If not, see <http://www.gnu.org * 
/licenses/>.
 */

/* 
 * File:   IrisDriver.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 3, 2021, 5:37 PM
 */

#include "Driver.h"
#include <interface/Iris.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <stdlib.h>

#ifdef _WIN32
  #include <windows.h>
  #define dlopen LoadLibrary 
  #define LibHandle HINSTANCE 
  #define dlsym GetProcAddress

  static inline LibHandle loadSharedObject( const char* input )
  {
    return dlopen( input ) ;
  }

  static inline const char* getError()
  {
    return "Windows Error handler not yet implemented." ;
  }

  static inline void releaseHandle( LibHandle handle )
  {
    FreeLibrary( handle ) ;
  }

#elif __linux__ 
  #include <dlfcn.h>
  #define LibHandle void* 

  static inline LibHandle loadSharedObject( const char* input )
  {
    return dlopen( input, RTLD_LAZY ) ;
  }
  
  static inline const char* getError()
  {
    return dlerror() ;
  }
  
  static inline void releaseHandle( LibHandle handle )
  {
    dlclose( handle ) ;
  }

#endif

/** Function to return the usage string.
 * @return The usage string for this executable.
 */  
static std::string usage() ;

Iris                    iris      ;
std::vector<std::string> dep_path   ;
std::string              setup_path ;


std::string usage()
{
  std::string tmp ;
  
  tmp = "Usage: iris_exe <IRIS_SETUP_JSON_PATH> <ADDITIONAL_DEPENDANCY_PATHS...>\n" 
        "       iris_exe <IRIS_SETUP_JSON_PATH>\n                                 " ;
  
  return tmp ;
}

void loadDependancies()
{
  for( auto str : dep_path )
  {
    for( auto& path : std::filesystem::recursive_directory_iterator( str.c_str(), std::filesystem::directory_options::follow_directory_symlink ) )
    {
      if( !std::filesystem::is_directory( path ) )
      {
        std::cout << "Loading Specified Dependancy: " << path.path().string() << std::endl ;
        
        loadSharedObject( path.path().string().c_str() ) ;
      }
    }
  }
}

IrisDriver::IrisDriver()
{
  setup_path = "" ;
}

IrisDriver::~IrisDriver()
{
  
}

void IrisDriver::initialize( unsigned argc, const char* arguments[] )
{
  std::string tmp ;
  
  if( argc == 1 )
  {
    std::cout << usage() << std::endl ;
    exit( 0 ) ;
  }
  for( unsigned i = 0; i < argc; i++ )
  {
    tmp = arguments[ i ] ;
    
    if( i == 1 )
    {
      setup_path = tmp ;
    }
    else if( i > 1 )
    {
      dep_path.push_back( tmp ) ;
      std::cout << tmp << std::endl ;
    }
  }
  
  loadDependancies() ;
}

int IrisDriver::run()
{
  iris.initialize( setup_path.c_str() ) ;
  return iris.run() ;
}

