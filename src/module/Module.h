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

#ifndef IRIS_MODULE
#define IRIS_MODULE

// Defines for different OS Shared Library Exports.
#ifdef _WIN32
  #include <windows.h>
  #define exported_function extern "C" __declspec( dllexport )
#else 
  #define exported_function extern "C"
#endif

namespace iris
{
  /** Class for describing a Module for use in the Iris Framework.
   */
  class Module
  { 
    public:
      /** Default Constructor. Initializes this object's data.
       */
      Module() ;
      
      /** Virtual deconstructor. Needed for inheritance.
       */
      virtual ~Module() ;
      
      /** Method to initialize this module after being configured.
       */
      virtual void initialize() = 0 ;
      
      /** Method to subscribe this module's configuration to the bus.
       * @param id The id to use for this graph.
       */
      virtual void subscribe( unsigned id ) ;

      /** Method to shut down this object's operation.
       */
      virtual void shutdown() = 0 ;
      
      /** Method to execute a single instance of this module's operation.
       */
      virtual void execute() = 0 ;
      
      /**  Method to retrieve the id of module in this graph.
       * @return The id of module in this graph.
       */
      unsigned id() const ;
      
      /** Method to set the id of this module.
       * @param id The id to associate with this module.
       */
      void setId( unsigned id ) ;

      /** Method to start operations of this module.
       */
      void start() ;
      
      /** Method to kick this module to start a single execution.
       */
      void kick() ;

      /** Method to stop operation of this module.
       * @return Whether the module is stopped or not.
       */
      bool stop() ;

      /** Method to set the version of this module.
       * @param version The version of this module.
       */
      void setVersion( unsigned version ) ;

      /** Method to set the name of this module.
       * @param name
       */
      void setName( const char* name ) ;
      
      /** Method to set the type name of this module.
       * @param type_name The name of the type of module this is.
       */
      void setTypeName( const char* name ) ;
      
      /** Method to retrieve the version of this module.
       * @return The version of this module
       */
      unsigned version() const ;
      
      /**
       * @return 
       */
      const char* type() const ;

      /** Method to retrieve the name of this module.
       * @return The name of this module.
       */
      const char* name() const ;
      
    private:
      
      /** Forward declared structure to contain this object's data.
       */
      struct ModuleData* module_data ;
      
      /** Method to retrieve a reference to this object's internal data structure.
       * @return Reference to this object's internal data structure.
       */
      ModuleData& data() ;

      /** Method to retrieve a const-reference to this object's internal data structure.
       * @return Const-reference to this object's internal data structure.
       */
      const ModuleData& data() const ;
  };
}
#endif