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

/* 
 * File:   Graph.h
 * Author: Jordan Hendl
 *
 * Created on July 10, 2020, 6:04 AM
 */

#ifndef IRIS_GRAPH_H
#define IRIS_GRAPH_H

namespace iris
{
  class Module ;
  class Loader ;
  class Graph
  {
    public:
      Graph() ;
      ~Graph() ;
      void initialize( Loader& mod_loader, const char* graph_config_path, unsigned id = 0 ) ;
      void add( const char* name, Module* module ) ;
      bool has( const char* name ) const ;
      const Module* module( const char* name ) ;
      void setName( const char* name ) ;
      void kick() ;
      void stop() ;
      void reset() ;
    private:
      struct GraphData* graph_data ;
      GraphData& data() ;
      const GraphData& data() const ;
  };
}

#endif

