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
#include <profiling/Timer.h>
#include <log/Log.h>
#include <data/Bus.h>
#include <string>
#include <map>
#include <thread>
#include <iostream>
#include <queue>
#include <thread>
#include <chrono>

namespace iris
{
  struct GraphData
  {
    using ModuleGraph     = std::map<std::string, Module*>  ;
    using Config          = iris::config::Configuration     ;
    using PriorityQueue   = std::vector<Module*>            ;
    using StringVec       = std::vector<std::string>        ;
    using InputOutputPair = std::pair<StringVec, StringVec> ;

    PriorityQueue   queue             ;
    iris::Timer     timer             ;
    iris::Bus       bus               ;
    Config          config            ;
    Loader*         loader            ;
    ModuleGraph     pre_graph         ;
    ModuleGraph     graph             ;
    unsigned        bus_id            ;
    std::string     graph_name        ;
    std::string     graph_config_path ;
    unsigned        id                ;
    bool            running           ;
    
    /** Method to shutdown all modules & clear the priority queue & graph.
     */
    void clear() ;
    
    /** Helper method to move prexisting modules to the newly generated graph in the event of a reload.
     */
    void movePrexisting() ;

    /** Method to configure a module.
     * @param token The JSON token at the specified module's location in the file.
     * @param name The name of the module.
     */
    void configureModule( iris::config::json::Token& token, std::string& name ) ;

    /** Helper method when solving the graph. Used for finding the inputs and outputs of a module.
     * @param token The JSON token to process.
     * @return A Pair of vectors of strings, in the format of < INPUTS, OUTPUTS >.
     */
    InputOutputPair findInputsAndOutputs( const iris::config::json::Token& token ) ;

    /** Method to kick off the module threads.
     */
    void kick() ;

    /** Method to load all modules in this graph.
     */
    void load() ;
    
    /** Helper method when solving the graph. Used for recursively traversing the graph and building a priority of each module.
     * @param module The module to retrieve a priority for.
     * @param count How deep this recursion is, used for breaking out of potential infinite loops in graphs.
     * @return The priority of the module, calculated as the amount of modules it has to traverse to get to the beginning.
     */
    unsigned recursiveSolve( Module* module, unsigned count = 0 ) ;

    /** Helper method to reload the config and repopulate the graph.
     */
    void reload() ;

    /** Method to stop the graph.
     */
    void stop() ;
    
    /** Helper method to solve the graph, and calculate a priority queue of the modules.
     */
    void solve() ;
    
    /** The main compute loop of this graph.
     */
    void traverse() ;
  };
  
  void GraphData::movePrexisting()
  {
    const auto token = this->config.begin()[ this->graph_name.c_str() ] ;
    iris::Module *module ;

    if( token )
    {
      // Search for modules.
      for( auto mod = token.begin(); mod != token.end(); ++mod )
      {
        for( unsigned index = 0; index < this->queue.size(); index++ )
        {
          module = this->queue[ index ] ;
          if( std::string( mod.key() ) == module->name() )
          {
            this->pre_graph.insert( { module->name(), module } ) ;
            this->queue.erase( this->queue.begin() + index ) ;
          }
        }
      }
    }
  }

  void GraphData::traverse()
  {
    while( this->running )
    {
      this->timer.start() ;
      if( this->config.modified() ) this->reload() ;
      for( auto module : this->queue )
      {
        module->kick() ;
      }
      
      while( !this->queue.back()->ready() ) std::this_thread::sleep_for( std::chrono::microseconds( 10 ) ) ;
      this->timer.stop() ;

      iris::log::Log::output( "Graph '", this->graph_name.c_str(), "' execution time: ", this->timer.output() ) ;
    }
  }

  void GraphData::clear()
  {
    std::string type ;
    for( auto node : this->queue )
    {
      type = node->type() ;
      
      node->shutdown() ;
      this->loader->descriptor( type.c_str() ).destroy( node ) ;
    }

    this->queue.clear() ;
    this->graph.clear() ;
  }

  void GraphData::kick()
  {
    for( auto module : this->queue )
    {
      if( this->pre_graph.find( module->name() ) == this->pre_graph.end() )
      {
        module->initialize() ;
      }

      std::thread( &Module::start, module ).detach() ;
    }
  }

  void GraphData::stop()
  {
    for( auto module : this->queue )
    {
      while( !module->stop() ) { module->kick() ; } ;
    }
  }

  GraphData::InputOutputPair GraphData::findInputsAndOutputs( const iris::config::json::Token& token )
  {
    GraphData::InputOutputPair pair ;
    std::string                key  ;

    for( auto& param : token )
    {
      key = param.key() ;
      
      if( key == "inputs" || key.find( "inputs" ) != std::string::npos )
      {
        if( param.isArray() )
        {
          for( unsigned i = 0; i < param.size(); i++ )
            pair.first.push_back( param.string( i ) ) ;
        }
        else
        {
          pair.first.push_back( param.string() ) ;
        }
      }
      
      else if( key == "outputs" || key.find( "outputs" ) != std::string::npos )
      {
        if( param.isArray() )
        {
          for( unsigned i = 0; i < param.size(); i++ )
            pair.second.push_back( param.string( i ) ) ;
        }
        else
        {
          pair.second.push_back( param.string() ) ;
        }
      }
    }
    
    return pair ;
  }

  unsigned GraphData::recursiveSolve( Module* module, unsigned count )
  {
    const iris::config::json::Token start = this->config.begin() ;
    unsigned priority ;

    if( count >= 300 )
    {
      iris::log::Log::output( iris::log::Log::Level::Fatal, "Possible loop found in graph. Exitting." ) ;
    }

    priority = 1 ;
    
    auto first = this->findInputsAndOutputs( start[ module->name() ] ) ;

    // For every other module, check and see if their output match the selected input.
    for( const auto& dep : this->graph )
    {
      
      auto second = this->findInputsAndOutputs( start[ dep.second->name() ] ) ;
      for( auto input : first.first )
      {
        for( auto output : second.second )
        {
          // If this module depends on the selected one's output, then add their priority to ours.
          if( input == output ) priority += this->recursiveSolve( dep.second, count++ ) ;
        }
      }
    }
    
    return priority ;
  }

  void GraphData::solve()
  {
    std::multimap<unsigned, Module*> map ;
    for( const auto& module : this->graph )
    {
      map.insert( { this->recursiveSolve( module.second ), module.second } ) ;
    }
    
    for( auto module : map )
    {
      this->queue.push_back( module.second ) ;
    }
    
    this->config.reset() ;
    this->graph.clear() ;
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
        if( param.isArray() )
        {
          for( unsigned index = 0; index < param.size(); index++ )
          {
            this->bus.emitIndexed( param.number ( index ), index, name.c_str(), "::", key.c_str() ) ;
            this->bus.emitIndexed( param.decimal( index ), index, name.c_str(), "::", key.c_str() ) ;
            this->bus.emitIndexed( param.string ( index ), index, name.c_str(), "::", key.c_str() ) ;
            this->bus.emitIndexed( param.boolean( index ), index, name.c_str(), "::", key.c_str() ) ;
          }
        }
        else
        {
          this->bus.emit( param.number (), name.c_str(), "::", key.c_str() ) ;
          this->bus.emit( param.decimal(), name.c_str(), "::", key.c_str() ) ;
          this->bus.emit( param.string (), name.c_str(), "::", key.c_str() ) ;
          this->bus.emit( param.boolean(), name.c_str(), "::", key.c_str() ) ;
        }
      }
    }
  }
  
  void GraphData::reload()
  {
//    iris::log::Log::output( "Graph ", this->graph_name.c_str(), " reloading..." ) ;
    this->stop()           ;
    this->config.reset()   ;
    this->config.initialize( this->graph_config_path.c_str() ) ;
    this->movePrexisting() ;
    this->clear()          ;

    this->load () ;
    this->solve() ;
    this->kick () ;
    this->pre_graph.clear() ;
//    iris::log::Log::output( "Graph ", this->graph_name.c_str(), " reloaded!" ) ;
  }

  void GraphData::load()
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
        
        if( this->graph.find( name ) == this->graph.end() && this->pre_graph.find( name ) == this->pre_graph.end() )
        {
          Log::output( "Initializing module. \n\n",
          "  - Name    : ", name.c_str()            , "\n",
          "  - Module  : ", type.c_str()            , "\n",
          "  - Version : ", version                 , "\n",
          "  - Graph   : ", this->graph_name.c_str(), "\n" ) ;
          
          auto module = loader->descriptor( type.c_str() ).create ( version ) ;
          
          if( module )
          {
            module->setName    ( name.c_str()     ) ;
            module->setVersion ( version          ) ;
            module->subscribe  ( this->id         ) ;
            this->graph.insert( { name, module }  ) ;
            
          }
          else
          {
            Log::output( Log::Level::Warning, "Failed to load module: ", name.c_str() ) ;
          }
        }
        
        this->configureModule( mod, name ) ;
      }
    }
    
    for( auto& iter : this->pre_graph )
    {
      this->graph.insert( iter ) ;
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

    data().config.initialize( graph_config_path ) ;
    data().load()  ;
    data().solve() ;
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
  
  void Graph::pulse()
  {
    for( auto& module : data().graph )
    {
      module.second->kick() ;
    }
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
    data().graph_name = name ;
  }

  void Graph::kick()
  {
    GraphData::PriorityQueue queue ;

    iris::log::Log::output( "Kicking off graph ", data().graph_name.c_str() ) ;
    data().running = true ;
    
    data().kick    () ;
    data().traverse() ;
    
    this->stop() ;
  }

  void Graph::stop()
  {
    data().stop() ;
  }

  void Graph::reset()
  {
    data().clear() ;
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