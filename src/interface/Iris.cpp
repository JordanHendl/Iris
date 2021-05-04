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

#include "Iris.h"
#include <log/Log.h>
#include <module/Manager.h>
#include <config/Configuration.h>
#include <config/Parser.h>
#include <data/Bus.h>
#include <string>
#include <vector>
#include <filesystem>
#include <stdlib.h>
#include <condition_variable>
#include <mutex>
#include <stdlib.h>

struct IrisData
{
  iris::Bus                   bus                ; ///< The bus to use for data transfer.
  iris::Manager               mod_manager        ; ///< The Graph module manager.
  iris::config::Configuration config             ; ///< The Configuration to initialize the library with.
  std::string                 module_path        ; ///< The path to the modules on the filesystem.
  std::string                 module_config_path ; ///< The path to the graph config on the filesystem.
  bool                        running            ; ///< Whether or not iris is running.
  std::condition_variable     cv                 ; ///< The condition variable to use for waiting to end.
  std::mutex                  mutex              ; ///< The mutex to wait on for an exit condition.
  bool                        use_log_stddout    ; ///< Whether or not to use stddout for logging.
  
  /** Default constructor.
   */
  IrisData() ;
  
  /** Method to act as a pathway for the graph to signal iris to shutdown.
   * @param exit Whether or not iris should exit.
   */
  void setExit( bool exit ) ;

  /** Method to set the graph configuration path.
   * @param path The path to the graph configuration on the filesystem.
   */
  void setModuleConfigPath( const char* path ) ;
  
  /** Method to set the path to all modules on the filesystem.
   * @param path Path to all modules on the filesystem to load.
   */
  void setModulePath( const char* path ) ; 
  
  /** Method to set whether or not the logs should be enabled or not.
   * @param output Whether or not logs should be enabled.
   */
  void setLogEnable( bool boolean ) ;

  /** Method to set the output directory of iris logs.
   * @param output The output directory of iris logs.
   */
  void setDebugOutput( const char* output ) ;
  
  /** Method to set the debug output mode of the iris logger.
   * @param output The debug output mode of the iris logger.
   */
  void setDebugMode( const char* output ) ;
  
  /** Method to parse setup information.
   */
  void parseSetup() ;
};

IrisData::IrisData()
{
  this->use_log_stddout = true  ;
  this->running         = false ;
}

void IrisData::setExit( bool exit )
{
  if( exit )
  {
    this->mod_manager.shutdown() ;
    this->running = false        ;
    iris::log::Log::flush()      ;
    this->cv.notify_all()        ;
//    ::exit( 0 ) ;
  }
}

void IrisData::setLogEnable( bool val )
{
  iris::log::Log::setEnabled( val ) ;
}

void IrisData::setDebugMode( const char* mode )
{
  using Log = iris::log::Log ;
  std::string tmp = mode ;
  
  if( tmp == "Verbose" )
  {
    Log::setMode( Log::Mode::Verbose ) ;
  }
  if( tmp == "Normal" )
  {
    Log::setMode( Log::Mode::Normal ) ;
  }
  if( tmp == "Quiet" )
  {
    Log::setMode( Log::Mode::Quiet ) ;
  }
}
void IrisData::setDebugOutput( const char* output )
{
  iris::log::Log::initialize( output, this->use_log_stddout ) ;
}

void IrisData::setModulePath( const char* path )
{
  this->module_path = path ;
}

void IrisData::setModuleConfigPath( const char* path )
{
  this->module_config_path = path ;
}

void IrisData::parseSetup()
{
  auto token = this->config.begin() ;
  
  auto graph_config = token[ "graph_config_path"   ] ;
  auto module_path  = token[ "module_path"         ] ;
  auto log_output   = token[ "log_output_path"     ] ;
  auto log_mode     = token[ "log_mode"            ] ;
  auto log_enable   = token[ "log_enable"          ] ;
  auto log_stddout  = token[ "log_use_stdout"      ] ;
  auto graph_timings= token[ "graph_timing_enable" ] ;
  
  if( graph_config  ) this->setModuleConfigPath              ( graph_config.string()   ) ;
  if( module_path   ) this->setModulePath                    ( module_path.string()    ) ;
  if( log_stddout   ) this->use_log_stddout =  log_stddout.boolean() ;
  if( log_output    ) this->setDebugOutput                   ( log_output.string()     ) ;
  if( log_mode      ) this->setDebugMode                     ( log_mode.string()       ) ;
  if( log_enable    ) this->setLogEnable                     ( log_enable.boolean()    ) ;
  if( graph_timings ) this->mod_manager.setEnableGraphTimings( graph_timings.boolean() ) ;
}

Iris::Iris()
{
  this->iris_data = new IrisData() ;
}

Iris::~Iris()
{
  delete this->iris_data ;
}

void Iris::shutdown()
{
  data().mod_manager.shutdown() ;
  iris::log::Log::flush()       ;
}

bool Iris::run()
{
  std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>( data().mutex ) ;
  data().cv.wait( lock, [=] { return !data().running ; } ) ;
  
  return data().running ;
}

void Iris::initialize( const char* setup_json_path )
{
  const std::string iris_config_path = setup_json_path ;
  
  // Set the exit condition in the event bus.
  data().bus.enroll( this->iris_data, &IrisData::setExit, iris::OPTIONAL, "Iris::Exit::Flag" ) ;
  
  data().config.initialize( iris_config_path.c_str() ) ;
  data().parseSetup() ;
  
  if( data().module_config_path.empty() )
  {
    iris::log::Log::output( "No module config path given in file ", setup_json_path, ". Unable to startup any graph. Exitting" ) ;
    iris::log::Log::flush() ;
    exit( -1 ) ;
  }
  
  if( data().module_path.empty() )
  {
    iris::log::Log::output( "No module path given in file ", setup_json_path, ". Cannot load any modules for the graph to use. Exitting." ) ;
    iris::log::Log::flush() ;
    exit( -1 ) ;
  }
  
  data().running = true ;
  data().mod_manager.initialize( data().module_path.c_str(), data().module_config_path.c_str() ) ;
  data().mod_manager.start() ;
  
  iris::log::Log::flush() ;
}

bool Iris::running() const
{
  return data().running ;
}

IrisData& Iris::data()
{
  return *this->iris_data ;
}

const IrisData& Iris::data() const
{
  return *this->iris_data ;
}
