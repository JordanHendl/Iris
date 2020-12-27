/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Graph.h
 * Author: jhendl
 *
 * Created on July 10, 2020, 6:04 AM
 */

#ifndef KARMA_GRAPH_H
#define KARMA_GRAPH_H

namespace karma
{
  class Module ;
  class Loader ;
  class Graph
  {
    public:
      Graph() ;
      ~Graph() ;
      void initialize( Loader* loader ) ;
      void add( const char* name, Module* module ) ;
      void subscribe( const char* name, unsigned id ) ;
      void setName( const char* name ) ;
      void kick() ;
      void load() ;
      void stop() ;
      void reset() ;
    private:
      struct GraphData* graph_data ;
      GraphData& data() ;
      const GraphData& data() const ;
  };
}

#endif

