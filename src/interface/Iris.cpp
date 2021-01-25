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

struct IrisData
{
  iris::Bus                   bus                ; ///< The bus to use for data transfer.
  iris::Manager               mod_manager        ; ///< The Graph module manager.
  iris::config::Configuration config             ; ///< The Configuration to initialize the library with.
  std::string                 module_path        ; ///< The path to the modules on the filesystem.
  std::string                 module_config_path ; ///< The path to the graph config on the filesystem.
  bool                        running            ; ///< Whether or not iris is running.
  
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
  this->running = false ;
}

void IrisData::setExit( bool exit )
{
  if( !exit )
  {
    this->mod_manager.shutdown() ;
    this->running = false ;
  }
}

void IrisData::setDebugMode( const char* mode )
{
  using Log = iris::log::Log ;
  std::string tmp = mode ;
  
  if( tmp == "VERBOSE" )
  {
    Log::setMode( Log::Mode::Verbose ) ;
  }
  if( tmp == "NORMAL" )
  {
    Log::setMode( Log::Mode::Normal ) ;
  }
  if( tmp == "QUIET" )
  {
    Log::setMode( Log::Mode::Quiet ) ;
  }
}
void IrisData::setDebugOutput( const char* output )
{
  iris::log::Log::initialize( output ) ;
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
  
  auto graph_config = token[ "graph_config_path" ] ;
  auto module_path  = token[ "module_path"       ] ;
  auto log_output   = token[ "log_output_path"   ] ;
  auto log_mode     = token[ "log_mode"          ] ;
  
  if( graph_config ) this->setModuleConfigPath( graph_config.string() ) ;
  if( module_path  ) this->setModulePath      ( module_path.string()  ) ;
  if( log_output   ) this->setDebugOutput     ( log_output.string()   ) ;
  if( log_mode     ) this->setDebugMode       ( log_mode.string()     ) ;
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
  iris::log::Log::flush()      ;
  data().mod_manager.shutdown() ;
}

void Iris::initialize( const char* setup_json_path )
{
  const std::string iris_config_path = setup_json_path ;
  
  // Set the exit condition in the event bus.
  data().bus.enroll( this->iris_data, &IrisData::setExit, "IRIS_EXIT_FLAG" ) ;
  
  data().config.initialize( iris_config_path.c_str(), 0 ) ;
  data().parseSetup() ;

  data().mod_manager.initialize( data().module_path.c_str(), data().module_config_path.c_str() ) ;
  data().mod_manager.start() ;
  
  iris::log::Log::flush() ;
  data().running = true ;
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
