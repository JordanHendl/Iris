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

#include "Manager.h"
#include "Loader.h"
#include "Graph.h"
#include <log/Log.h>
#include <data/Bus.h>
#include <config/Configuration.h>
#include <config/Parser.h>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <thread>

namespace iris
{
  struct ManagerData
  {
    typedef unsigned                      Version    ;
    typedef std::map<Version, Module*>    ModuleMap  ;
    typedef std::map<std::string, Graph*> NodeGraphs ;
    
    iris::config::Configuration config ;
    
    std::map<std::string, std::thread> graph_threads ;
    bool                               graph_timings ;
    std::string                        config_path   ;
    std::string                        mod_path      ;
    Loader                             loader        ;
    NodeGraphs                         graphs        ;
    std::mutex                         lock          ;
    
    /** Constructor.
     */
    ManagerData() ;
    
    /** Method to find all graphs in the configuration.
     */
    void findGraphs() ;
    
    /** Method to add a graph.
     * @param name The name of the graph to add.
     */
    void addGraph( const char* name ) ;
  };
  
  ManagerData::ManagerData()
  {
    this->graph_timings = false ;
    this->config_path   = ""    ;
    this->mod_path      = ""    ;
  }

  void ManagerData::findGraphs()
  {
    this->config.initialize( this->config_path.c_str() ) ;
    
    auto token = this->config.begin() ;
    
    // For every graph in the top level, create the object.
    for( auto graph = token.begin(); graph != token.end(); ++graph )
    {
      this->addGraph( graph.key() ) ;
    }
  }
  void ManagerData::addGraph( const char* name )
  {
    if( this->graphs.find( name ) == this->graphs.end() )
    {
      iris::log::Log::output( "Adding graph ", name ) ;
      Graph* graph ;
      
      graph = new Graph() ;
      
      graph->setEnableTimings( this->graph_timings                                          ) ;
      graph->setName         ( name                                                         ) ;
      graph->initialize      ( this->loader, this->config_path.c_str(), this->graphs.size() ) ;
      this->graphs.insert    ( { name, graph }                                              ) ;
    }
  }

  Manager::Manager()
  {
    this->man_data = new ManagerData() ;
  }

  Manager::~Manager()
  {
    delete this->man_data ;
  }
  
  void Manager::setEnableGraphTimings( bool val )
  {
    data().graph_timings = val ;
  }
  
  void Manager::initialize( const char* mod_path, const char* configuration_path )
  {
    data().config_path = configuration_path ;
    
    iris::log::Log::output( "Initializing Module Manager with modules in ", mod_path, " using the following config: ", configuration_path ) ;

    data().loader.initialize( mod_path ) ;
    data().findGraphs() ;
  }

  void Manager::start()
  {
    unsigned index ;
    
    index = 0 ;
    
    iris::log::Log::output( "Initializing all current active graphs." ) ;

    for( auto &graph : data().graphs ) 
    {
      std::thread thread( &iris::Graph::kick, graph.second ) ;
      data().graph_threads[ graph.first ] = std::move( thread ) ;

      index++ ;
    }
  }
  
  void Manager::stop()
  {
    for( auto& graph : data().graphs )
    {
      graph.second->stop() ;
    }

  }
  
  void Manager::shutdown()
  {
    for( auto graph : data().graphs )
    {
      graph.second->stop () ;
      graph.second->reset() ;
    }
    
    for( auto& thread : data().graph_threads )
    {
      thread.second.join() ;
    }
  }

  ManagerData& Manager::data()
  {
    return *this->man_data ;
  }

  const ManagerData& Manager::data() const
  {
    return *this->man_data ;
  }
}