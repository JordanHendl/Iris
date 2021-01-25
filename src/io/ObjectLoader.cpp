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

#include "ObjectLoader.h"
#include <map>
#include <iostream>

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


namespace iris
{
  namespace io
  {
    struct ObjectLoaderData
    {
      typedef std::map<std::string, ObjectLoader::DL_FUNC> FunctionMap ;

      LibHandle   handle ;
      FunctionMap map    ;

      ObjectLoaderData()
      {
        this->handle = NULL ;
      }
    };

    ObjectLoader::ObjectLoader()
    {
      this->loader_data = new ObjectLoaderData() ;
    }

    ObjectLoader::~ObjectLoader()
    {
      delete this->loader_data ;
    }

    ObjectLoader::DL_FUNC ObjectLoader::symbol( const char* symbol_name )
    {
      ObjectLoader::DL_FUNC func ;

      if( data().map.find( symbol_name ) == data().map.end() )
      {
        func = reinterpret_cast<ObjectLoader::DL_FUNC>( dlsym( data().handle, symbol_name ) ) ;
        if( func ) data().map.insert( { symbol_name, func } ) ;
        else       std::cout << "Symbol not found: " << symbol_name << std::endl ;
      }

      return data().map.at( symbol_name ) ;
    }

    void ObjectLoader::load( const char* lib_path )
    {
      data().handle = loadSharedObject( lib_path ) ;

      if( data().handle == nullptr )
      {
        std::cout << "Error loading shared object " << lib_path << ". Error: " << ::getError() << std::endl  ;
      }
    }
    
    void ObjectLoader::reset()
    {
      if( data().handle ) ::releaseHandle( data().handle ) ;
    }

    ObjectLoaderData& ObjectLoader::data()
    {
      return *this->loader_data ;
    }

    const ObjectLoaderData& ObjectLoader::data() const
    {
      return *this->loader_data ;
    }
  }
}
