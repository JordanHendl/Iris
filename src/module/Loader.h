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

#ifndef IRIS_LOADER_H
#define IRIS_LOADER_H

namespace iris
{
  class Module ;
  
  /** Class to manage creation and destruction of a module.
   */
  class Descriptor
  {
    public:
      
      /** Default constructor.
       */
      Descriptor() ;
      
      /** Deconstructor.
       */
      ~Descriptor() ;
      
      /** Copy constructor. Copies the inputs contents into this object.
       * @param desc The descriptor object to copy.
       */
      Descriptor( const Descriptor& desc ) ;
      
      /** Assignment operator. Assigns this object to the input.
       * @param desc The descriptor object to assign this one to.
       * @return Reference to this object after assignment.
       */
      Descriptor& operator=( const Descriptor& desc ) ;
      
      /** Method to create a module and retrieve the pointer.
       * @param version The version of module to create. Defaults to latest.
       * @return The newly created module.
       */
      Module* create( unsigned version = 0 ) ;
      
      /** Method to destroy a module.
       * @param module The pointer to the module to destroy.
       * @param version The version of module.
       */
      void destroy( Module* module, unsigned version = 0 ) const ;

    private:
      
      /** Friend decleration.
       */
      friend struct LoaderData ;
      
      /** Method to initialize this descriptor.
       * @param module_path The path to the module to load on the filesystem.
       */
      void initalize( const char* module_path ) ;

      /** The forward declared structure containing this object's data.
       */
      struct DescriptorData* desc_data ;
      
      /** Method to retrieve a reference to this object's internal data structure.
       * @return A reference to this object's internal data structure.
       */
      DescriptorData& data() ;
      
      /** Method to retrieve a reference to this object's internal data structure.
       * @return A reference to this object's internal data structure.
       */
      const DescriptorData& data() const ;
  };

  /** Class to manage loading of all modules.
   */
  class Loader
  {
    public:
      
      /** Default constructor.
       */
      Loader() ;
      
      /** Deconstructor.
       */
      ~Loader() ;
      
      /** Copy constructor. Copies the input into this object.
       * @param loader The object to copy data from.
       */
      Loader( const Loader& loader ) ;
      
      /** Assignment operator. Assigns this object to the input.
       * @param loader The object to assign this one to.
       * @return Reference to this object after assignment.
       */
      Loader& operator=( const Loader& loader ) ;
      
      /** Method to initialize this object.
       * @param module_path The path to all modules to load on the filesystem.
       */
      void initialize( const char* module_path ) ;
      
      /** Method to recieve a descriptor of the input module type.
       * @param module_type The type of module to recieve a descriptor for.
       * @return A const reference to a descriptor to assist in module creation.
       */
      const Descriptor& descriptor( const char* module_type ) const ;
      
      /** Method to recieve a descriptor of the input module type.
       * @param module_type The type of module to recieve a descriptor for.
       * @return A const reference to a descriptor to assist in module creation.
       */
      Descriptor& descriptor( const char* module_type ) ;
      
      /** Method to check if a descriptor exists for the given module type.
       * @param module_type The type of module to check.
       * @return Whether or not this type of module is found in this loader's data.
       */
      bool hasDescriptor( const char* module_type ) const ;
      
      /** Method to reset this object and free all allocated & loaded data.
       */
      void reset() ;
    private:
      /** The forward declared structure containing this object's data.
       */
      struct LoaderData *loader_data ;
      /** Method to retrieve a reference to this object's internal data structure.
       * @return A reference to this object's internal data structure.
       */
      LoaderData& data() ;
      /** Method to retrieve a reference to this object's internal data structure.
       * @return A reference to this object's internal data structure.
       */
      const LoaderData& data() const ;
  };
}

#endif

