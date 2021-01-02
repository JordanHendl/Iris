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

#include "Module.h"
#include <data/Bus.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#ifdef _WIN32
  #include <windows.h>

  static inline void setThreadPriority()
  {
    SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL ) ;
  }

#elif __linux__ 
  #include <pthread.h>
#include <sys/resource.h>

  static inline void setThreadPriority()
  {
    // TODO because I don't think this is doable without writing my own thread library which is stupid. Why tf can't std thread handle this?
  }
#endif
  
namespace karma
{
  /** Structure to contain the Module object's internal data.
   */
  struct ModuleData
  {
    typedef bool Flag ;

    std::string name       ; ///< The name of this module.
    std::string type       ; ///< The type of module this object is.
    unsigned    version    ; ///< The version of module.
    Flag        running    ; ///< Whether or not this module is running.
    Flag        should_run ; ///< Whether or not this module should be running.
    karma::Bus  bus        ; ///< The bus to communicate data over.
    unsigned    id         ; ///< The id associated with this module.

    /**
     */
    ModuleData() ;
    
    bool dependanciesMet() ;
  };
  
  ModuleData::ModuleData()
  {
    this->name       = ""    ;
    this->running    = false ;
    this->should_run = false ;
    this->id         = 0     ;
  }

  Module::Module()
  {
    this->module_data = new ModuleData() ;
  }
  
  Module::~Module()
  {
    delete this->module_data ;
  }
  
  void Module::subscribe( const char* pipeline, unsigned id )
  {
    data().bus.setChannel( id ) ;
  }
          
  void Module::start()
  {
    
    data().running    = true ;
    data().should_run = true ;
    
    setThreadPriority() ;
    
    while( data().should_run )
    {
      this->execute() ;
    }
    
    data().running = false ;
  }

  bool Module::stop()
  {
    data().should_run = false ;
    
    return data().running ; // TODO fix this, shutdown is borken.
  }
 
  void Module::setName( const char* name )
  {
    data().name = name ;
  }
  
  void Module::setVersion( unsigned version )
  {
    data().version = version ;
  }
  
  void Module::setTypeName( const char* name )
  {
    data().type = name ;
  }
  
  unsigned Module::id() const
  {
    return data().id ;
  }
  
  void Module::setId( unsigned id )
  {
    data().id = id ;
  }

  const char* Module::type() const
  {
    return data().type.c_str() ;
  }
  
  unsigned Module::version() const
  {
    return data().version ;
  }

  const char* Module::name() const
   {
    return data().name.c_str() ;
  }

  ModuleData& Module::data()
  {
    return *this->module_data ;
  }

  const ModuleData& Module::data() const
  {
    return *this->module_data ;
  }
}
