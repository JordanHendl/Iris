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
#include <condition_variable>

#ifdef _WIN32
  #define NOMINMAX // So we can use std::min/max and not have to do crazy bullshit just for windows.
  #include <windows.h>

  static inline void setThreadPriority()
  {
    SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL ) ;
  }

#elif __linux__ 
  #include <pthread.h>
#include <sys/resource.h>
#include <condition_variable>

  static inline void setThreadPriority()
  {
    // TODO because I don't think this is doable without writing my own thread library which is stupid. Why tf can't std thread handle this?
  }
#endif
  
namespace iris
{
  /** Structure to contain the Module object's internal data.
   */
  struct ModuleData
  {
    typedef bool Flag ;

    std::string       name        ; ///< The name of this module.
    std::string       type        ; ///< The type of module this object is.
    unsigned          version     ; ///< The version of module.
    Flag              running     ; ///< Whether or not this module is running.
    Flag              should_run  ; ///< Whether or not this module should be running.
    iris::Bus         bus         ; ///< The bus to communicate data over.
    unsigned          id          ; ///< The id associated with this module.
    std::mutex        mutex       ; ///< The mutex to use for locking.
    std::atomic<bool> is_signaled ; ///< Whether or not this module is signaled.

    std::condition_variable cv ;

    /**
     */
    ModuleData() ;
  };
  
  ModuleData::ModuleData()
  {
    this->name        = ""    ;
    this->running     = false ;
    this->should_run  = false ;
    this->id          = 0     ;
    this->is_signaled = false ;
  }

  Module::Module()
  {
    this->module_data = new ModuleData() ;
  }
  
  Module::~Module()
  {
    delete this->module_data ;
  }
          
  void Module::start()
  {
    setThreadPriority() ;
    
    data().should_run = true ;
    data().running    = true ;
    while( data().should_run )
    {
      std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>( data().mutex ) ;
      data().is_signaled = false ;
      data().cv.wait( lock, [=] { return data().is_signaled.load() ; } ) ;
      if( !data().should_run ) { data().running = false ; return ; }
      this->execute() ;
    }
  }
  
  void Module::kick()
  {
    {
      std::lock_guard<std::mutex> lock( data().mutex ) ;
      data().is_signaled = true ;
    }

    data().cv.notify_one() ;
  }
  
  bool Module::stop()
  {
    data().should_run = false ;
    
    return !data().running ; // TODO fix this, shutdown is borken.
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
  
  bool Module::ready() const
  {
    return !data().is_signaled ;
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
