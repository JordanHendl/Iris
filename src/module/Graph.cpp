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

#include "Graph.h"
#include "Module.h"
#include "Loader.h"
#include <config/Configuration.h>
#include <config/Parser.h>
#include <log/Log.h>
#include <data/Bus.h>
#include <string>
#include <map>
#include <thread>
#include <iostream>
namespace iris
{
  struct GraphData
  {
    typedef std::map<std::string, Module*> ModuleGraph ;
    using Config = iris::config::Configuration ;  

    iris::Bus  bus               ;
    Config      config            ;
    Loader*     loader            ;
    ModuleGraph graph             ;
    unsigned    bus_id            ;
    std::string graph_name        ;
    std::string graph_config_path ;
    unsigned    id                ;

    void stop  ( const char* name ) ;
    void remove( const char* name ) ;
    void setName( const char* name ) ;
    void loadModules() ;
    void configureModule( iris::config::json::Token& token, std::string& name ) ;
  };

  void GraphData::setName( const char* name )
  {
    this->graph_name = name ;
  }

  void GraphData::configureModule( iris::config::json::Token& token, std::string& name )
  {
    this->bus.setChannel( this->id ) ;
    
    std::string key ;
    
    for( auto param = token.begin(); param != token.end(); ++param )
    {
      key = param.key() ;
      if( key != "type" && key != "version" )
      {
        this->bus.emit( param.number (), name.c_str(), "::", key.c_str() ) ;
        this->bus.emit( param.decimal(), name.c_str(), "::", key.c_str() ) ;
        this->bus.emit( param.string (), name.c_str(), "::", key.c_str() ) ;
      }
    }
  }

  void GraphData::loadModules()
  {
    using namespace iris::log ;

    auto token = this->config.begin() ;
    std::string name    ;
    std::string type    ;
    std::string param   ;
    unsigned    version ;
    
    // Look up this graph
    auto graph = token[ this->graph_name.c_str() ] ;
    
    if( graph )
    {
      // Search for modules.
      for( auto mod = graph.begin(); mod != graph.end(); ++mod )
      {
        name = mod.key() ;
        version = 0  ;
        type    = "" ;

        for( auto params = mod.begin(); params != mod.end(); ++params )
        {
          param = params.key() ;
          
          if( param == "type"    ) type    = params.string() ;
          if( param == "version" ) version = params.number() ; 
       }
        
        if( this->graph.find( name ) == this->graph.end() )
        {
          Log::output( "Initializing module. \n\n",
          "  - Name    : ", name.c_str()            , "\n",
          "  - Module  : ", type.c_str()            , "\n",
          "  - Version : ", version                 , "\n",
          "  - Graph   : ", this->graph_name.c_str(), "\n" ) ;
          
          auto descriptor = loader->descriptor( type.c_str() ) ;  
          auto module     = descriptor.create ( version      ) ;
          
          if( module )
          {
            module->setName    ( name.c_str()     ) ;
            module->setVersion ( version          ) ;
            module->subscribe  ( this->id         ) ;
            this->graph.insert( { name, module } ) ;
            this->configureModule( mod, name ) ;
          }
          else
          {
            Log::output( Log::Level::Warning, "Failed to load module: ", name.c_str() ) ;
          }
        }
      }
    }
  }
  
  void GraphData::stop  ( const char* name )
  {
    auto iter = this->graph.find( name ) ;
    
    if( iter != this->graph.end() )
    {
      while( !iter->second->stop() ) {} ;
    }
  }

  void GraphData::remove( const char* name )
  {
    auto iter = this->graph.find( name ) ;

    if( iter != this->graph.end() )
    {
      this->stop( name ) ;
      this->graph.erase( iter ) ;
    }
  }

  Graph::Graph()
  {
    this->graph_data = new GraphData() ;
  }

  Graph::~Graph()
  { 
    delete this->graph_data ;
  }

  void Graph::initialize( Loader& mod_loader, const char* graph_config_path, unsigned id )
  {
    data().loader            = &mod_loader       ;
    data().graph_config_path = graph_config_path ;
    data().id                = id                ;
    data().config.initialize( graph_config_path, id ) ;
    data().loadModules() ;
  }
  
  bool Graph::has( const char* name ) const
  {
    auto iter = data().graph.find( name ) ;
    
    if( iter != data().graph.end() ) return true ;
    return false ;
  }
  
  const Module* Graph::module( const char* name )
  {
    auto iter = data().graph.find( name ) ;
    
    if( iter != data().graph.end() ) return iter->second ;
    return nullptr ;
  }
  
  void Graph::add( const char* name, Module* module )
  {
    if( data().graph.find( name ) == data().graph.end() )
    {
      data().graph.insert( { name, module } ) ;
    }
  }
  
  void Graph::setName( const char* name )
  {
    data().setName( name ) ;
  }

  void Graph::kick()
  {
    for( auto module : data().graph )
    {
      module.second->initialize() ;
      std::thread( &Module::start, module.second ).detach() ;
    }
  }

  void Graph::stop()
  {
    for( auto module : data().graph )
    {
      while( !module.second->stop() ) {} ;
    }
  }

  void Graph::reset()
  {
    std::string type ;
    for( auto node : data().graph )
    {
      type = node.second->type() ;

      auto descriptor = data().loader->descriptor( type.c_str() ) ;
      descriptor.destroy( node.second ) ;
    }

    data().graph.clear() ;
  }

  GraphData& Graph::data()
  {
    return *this->graph_data ;
  }

  const GraphData& Graph::data() const
  {
    return *this->graph_data ;
  }
}