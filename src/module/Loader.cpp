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

#define NOMINMAX
#include "Loader.h"
#include "Module.h"
#include <io/ObjectLoader.h>
#include <log/Log.h>
#include <filesystem>
#include <string> 
#include <algorithm>
#include <map>

namespace iris
{
  static inline bool valid( std::string path ) ;
  
  struct DescriptorData
  { 
    typedef unsigned Version ;
    using VersionFunc = iris::io::Symbol<unsigned>                 ;
    using MakeFunc    = iris::io::Symbol<iris::Module*, unsigned>  ;
    using NameFunc    = iris::io::Symbol<const char*>              ;
    using DestroyFunc = iris::io::Symbol<void, iris::Module*>      ;
    
    struct Module
    {
      VersionFunc version ;
      MakeFunc    make    ;
      NameFunc    name    ;
      DestroyFunc destroy ;
    };
    
    typedef std::map<Version, Module> ModuleMap ;
    
    ModuleMap              modules ;
    Version                latest  ;
    iris::io::ObjectLoader loader  ;
    
  };
  
  struct LoaderData
  {
    typedef std::map<std::string, Descriptor*> DescriptorMap ;
          
    DescriptorMap descriptor_map   ;
    std::string   module_directory ;
    
    void loadAllModules() ;
    void addModule( const char* path ) ;
  };
  
  bool valid( std::string path )
  {
    return ( path.find( ".dll" ) != std::string::npos ) || ( path.find( ".so" ) != std::string::npos ) ;
  }

  void LoaderData::loadAllModules()
  {
    iris::log::Log::output( "Loading modules at location: ", this->module_directory.c_str() ) ;

    for( auto& path : std::filesystem::recursive_directory_iterator( this->module_directory.c_str() , std::filesystem::directory_options::follow_directory_symlink ) )
    {
      if( iris::valid( path.path().string() ) )
      {
        this->addModule( path.path().string().c_str() ) ;
      }
    }
  }
  
  void LoaderData::addModule( const char* path )
  {
    iris::io::ObjectLoader         loader   ;
    iris::io::Symbol<const char*>  namefunc ;
    std::string                    name     ;

    loader.load( path ) ;
    namefunc = loader.symbol( "name" ) ;
    name = namefunc() ;
    
    loader.reset() ;

    if( this->descriptor_map.find( name ) == this->descriptor_map.end() )
    {
      this->descriptor_map.insert( { name, new Descriptor() } ) ;
    }
    
    this->descriptor_map.at( name )->initalize( path ) ;
  }

  Descriptor::Descriptor()
  {
    this->desc_data = new DescriptorData() ;
  }

  Descriptor::~Descriptor()
  {
    delete this->desc_data ;
  }

  Descriptor::Descriptor( const Descriptor& desc )
  {
    this->desc_data = new DescriptorData() ;
    *this->desc_data = *desc.desc_data ;
  }

  Descriptor& Descriptor::operator=( const Descriptor& desc )
  {
    *this->desc_data = *desc.desc_data ;
    
    return *this ;
  }
  
  void Descriptor::initalize( const char* module_path )
  {
    using namespace iris::log ;
    DescriptorData::Module mod ;
    unsigned version ;

    data().loader.load( module_path ) ;
    
    Log::output( "Loading shared library at: ", module_path ) ;

    mod.name    = data().loader.symbol( "name"    ) ;
    mod.make    = data().loader.symbol( "make"    ) ;
    mod.destroy = data().loader.symbol( "destroy" ) ;
    mod.version = data().loader.symbol( "version" ) ;
    
    version = mod.version() ;
    
    data().latest = std::max( data().latest, version ) ;
    
    if( mod.name && mod.make && mod.destroy && mod.version && data().modules.find( version ) == data().modules.end() )
    { 
      data().modules.insert( { version, mod } ) ;
    }
  }

  Module* Descriptor::create( unsigned version )
  {
    static Module* mod = nullptr ;
    
    if( version == 0 ) version = data().latest ;
    auto iter = data().modules.find( version ) ;
    
    return iter != data().modules.end() ? iter->second.make( version ) : mod ;
  }

  void Descriptor::destroy( Module* module, unsigned version )
  {
    if( module )
    {
      auto iter = data().modules.find( version ) ;
      
      if( version == 0 ) version = data().latest ;

      if( iter != data().modules.end() )
      {
        iter->second.destroy( module ) ;
      }
      else
      {
        delete module ;
      }
    }
  }

  DescriptorData& Descriptor::data()
  {
    return *this->desc_data ;
  }

  const DescriptorData& Descriptor::data() const
  {
    return *this->desc_data ;
  }

  Loader::Loader()
  {
    this->loader_data = new LoaderData() ;
  }

  Loader::~Loader()
  {
    delete this->loader_data ;
  }

  Loader::Loader( const Loader& loader )
  {
    this->loader_data = new LoaderData() ;
    *this = loader ;
  }
  
  Loader& Loader::operator=( const Loader& loader )
  {
    *this->loader_data = *loader.loader_data ;
    
    return *this ;
  }

  void Loader::initialize( const char* module_path )
  {
    data().module_directory = module_path ;
    data().loadAllModules() ;
  }

  const Descriptor& Loader::descriptor( const char* module_type ) const
  {
    static const Descriptor dummy ;
    auto iter = data().descriptor_map.find( module_type ) ;
    
    if( iter != data().descriptor_map.end() ) 
    {
      return *iter->second ;
    }
    
    return dummy ;
  }

  bool Loader::hasDescriptor( const char* module_type ) const
  {
    return data().descriptor_map.find( module_type ) != data().descriptor_map.end() ;
  }
  
  void Loader::reset()
  {
    for( auto desc: data().descriptor_map )
    {
      delete desc.second ;
    }

    data().descriptor_map.clear() ;
  }

  LoaderData& Loader::data()
  {
    return *this->loader_data ;
  }

  const LoaderData& Loader::data() const
  {
    return *this->loader_data ;
  }
}
