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
#include <log/Log.h>
#include <data/Bus.h>
#include <string>
#include <map>
#include <thread>
#include <iostream>

namespace karma
{
  struct NodeLoader
  {
    struct NodeInfo
    {
      typedef std::map<std::string, Module*> ModuleGraph ;
      std::string  name     ;
      std::string  type     ;
      std::string  pipeline ;
      unsigned     version  ;
      unsigned     id       ;
      ModuleGraph* graph    ;
      Loader*      loader   ;
      
      void setVersion( unsigned version )
      {
        this->version = version ;
      }

      void setType( const char* type )
      {
          this->type = type ;
      }
      
      void create()
      {
        using namespace karma::log ;

        if( this->graph->find( this->name ) == this->graph->end() )
        { 
          Log::output( " Initializing module. \n\n",

          "  - Name    : ", this->name.c_str()    , "\n",
          "  - Module  : ", this->type.c_str()    , "\n",
          "  - Version : ", this->version         , "\n",
          "  - Pipeline: ", this->pipeline.c_str(), "\n" ) ;

          auto descriptor = loader->descriptor( this->type.c_str() ) ;  
          auto module     = descriptor.create ( this->version      ) ;
          
          if( module )
          {
            module->setName    ( this->name.c_str()               ) ;
            module->setVersion ( this->version                    ) ;
            module->subscribe  ( this->id                         ) ;
            this->graph->insert( { this->name, module }           ) ;
          }
        }
      }
    };

    typedef std::map<std::string, Module*> ModuleGraph ;
    
    ModuleGraph*                    graph      ;
    Loader*                         loader     ;
    karma::Bus                      bus        ;
    std::map<std::string, NodeInfo> loaded_map ;
    std::string                     pipeline   ;

    NodeLoader()
    {
    }
    
    void load()
    {
      for( auto thing : this->loaded_map )
      {
        thing.second.create() ;
      }
    }
    
    void setName( const char* name )
    {
      std::string dependency ;

      dependency = this->pipeline + name ;
      
      if( this->loaded_map.find( name ) == this->loaded_map.end() )
      {
        
        this->bus.enroll( &this->loaded_map[ name ], &NodeInfo::setVersion, "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::version" ) ;
        this->bus.enroll( &this->loaded_map[ name ], &NodeInfo::setType   , "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::type"    ) ;

        this->loaded_map[ name ].graph    = this->graph    ;
        this->loaded_map[ name ].loader   = this->loader   ;
        this->loaded_map[ name ].name     = name           ;
        this->loaded_map[ name ].pipeline = this->pipeline ;

      }
    }
  };

  struct GraphData
  {
    typedef std::map<std::string, Module*> ModuleGraph ;
      
    karma::Bus  bus         ;
    ModuleGraph graph       ;
    Loader*     loader      ;
    NodeLoader  node_loader ;
    unsigned    bus_id      ;
    std::string window      ;
    
    void setWindow( const char* name ) ;
    void stop  ( const char* name ) ;
    void remove( const char* name ) ;
    void setName( const char* name ) ;
  };

  void GraphData::setName( const char* name )
  {
    std::string dependency ;
     
    dependency = std::string( name ) + "::add" ;

    this->node_loader.pipeline = name ;
    
    this->bus.enroll( this, &GraphData::remove, name, "::remove"                             ) ;
    this->bus.enroll( this, &GraphData::stop  , name, "::stop"                               ) ;
    this->bus.enroll( this, &GraphData::setWindow, "Graphs::", name, "::window"              ) ;
    this->bus.enroll( &this->node_loader, &NodeLoader::setName, "Graphs::", name, "::Modules" ) ;
  }

  void GraphData::setWindow(const char* name) 
  {
    this->window = name ;
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

   void Graph::initialize( Loader* loader )
  {
    data().loader = loader ; //todo remove this

    data().node_loader.loader =  data().loader ;
    data().node_loader.graph  = &data().graph ;
    data().node_loader.bus.setChannel( data().bus_id ) ;
    data().node_loader.loaded_map.clear() ;
  }

  void Graph::add( const char* name, Module* module )
  {
    if( data().graph.find( name ) == data().graph.end() )
    {
      data().graph.insert( {name, module } ) ;
    }
  }
  
  void Graph::setName( const char* name )
  {
    data().setName( name ) ;
  }

  void Graph::subscribe( const char* name, unsigned id )
  {
    data().node_loader.graph = &data().graph ;
  }
  
  void Graph::load()
  {
    data().node_loader.load() ;
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