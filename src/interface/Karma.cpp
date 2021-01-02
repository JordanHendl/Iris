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

#include "Karma.h"
#include <log/Log.h>
#include <node/Manager.h>
#include <config/Configuration.h>
#include <config/Parser.h>
#include <data/Bus.h>
#include <string>
#include <vector>
#include <filesystem>
#include <stdlib.h>

struct KarmaData
{
  ::karma::Bus                   bus                ; ///< The bus to use for data transfer.
  ::karma::Manager               mod_manager        ; ///< The Graph module manager.
  ::karma::config::Configuration config             ; ///< The Configuration to initialize the library with.
  std::string                    window             ; ///< The name of the current active window.
  std::string                    module_path        ; ///< The path to the modules on the filesystem.
  std::string                    module_config_path ; ///< The path to the modules on the filesystem.
  std::string                    database_path      ; ///< The path to the modules on the filesystem.
  bool                           running            ; ///< Whether or not karma is running.
  
  KarmaData() ;
  void fixString( std::string& str ) ;
  void setModuleConfigPath( const char* path ) ;
  void setModulePath( const char* path ) ; 
  void setDebugOutput( const char* output ) ;
};

KarmaData::KarmaData()
{
  this->running = false ;
}

void KarmaData::setDebugOutput( const char* output )
{
  karma::log::Log::initialize( output ) ;
}

void KarmaData::setModulePath( const char* path )
{
  this->module_path = path ;
}

void KarmaData::setModuleConfigPath( const char* path )
{
  this->module_config_path = path ;
}

void KarmaData::fixString( std::string& str )
{
  const char c = str.back() ;

  if( c == '\\' || c == '/' )
  {
    str.pop_back() ;
  }
}

Karma::Karma()
{
  this->karma_data = new KarmaData() ;
}

Karma::~Karma()
{
  delete this->karma_data ;
}

void Karma::shutdown()
{
  karma::log::Log::write()      ;
  data().mod_manager.shutdown() ;
}

void Karma::initialize( const char* setup_json_path )
{
  using namespace karma::log ;
  
  std::string karma_config_path ;
  
  data().bus.enroll( this->karma_data, &KarmaData::setModulePath      , "modules_path"      ) ;
  data().bus.enroll( this->karma_data, &KarmaData::setModuleConfigPath, "graph_config_path" ) ;
  data().bus.enroll( this->karma_data, &KarmaData::setDebugOutput     , "log_output_path"   ) ;

  karma_config_path = setup_json_path ;
  
  data().config     .initialize( karma_config_path.c_str(), 0                                    ) ;
  data().config     .initialize( data().database_path.c_str()  , 0                             ) ;
  data().mod_manager.initialize( data().module_path.c_str(), data().module_config_path.c_str() ) ;
  data().mod_manager.start() ;
  
  karma::log::Log::write() ;
  data().running = true ;
}

bool Karma::running() const
{
  return data().running ;
}

KarmaData& Karma::data()
{
  return *this->karma_data ;
}

const KarmaData& Karma::data() const
{
  return *this->karma_data ;
}
