/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Configuration.cpp
 * Version      : 1.0
 * Date created : 6/21/2020
 * Purpose      : Definitions for Configuration object to configure an entire program pipeline.
**********************************************************************/
#include "Configuration.h"
#include "Parser.h"
#include <log/Log.h>
#include <Bus.h>
#include <fstream>
#include <istream>
#include <iostream>

namespace karma
{
  namespace config
  {
    /** The data structure to contain all of the Configuration object's data.
     */
    struct ConfigurationData
    {
      karma::Bus                  bus    ; ///< The bus to sent data over.
      karma::config::json::Parser parser ; ///< The parser to use to parse the configuration.
      json::Token                 begin  ; ///< The beginning of this object's internal parsed data.
      json::Token                 end    ; ///< The end of this object's internal parsed data.
      bool                        init   ; ///< Whether or not this object is initialized.
      
      /** Default Constructor.
      */
      ConfigurationData() ;
    };

    ConfigurationData::ConfigurationData()
    {
      this->init = false ;
    }

    Configuration::Configuration()
    {
      this->config_data = new ConfigurationData() ;
    }

    Configuration::~Configuration()
    {
      delete this->config_data ;
    }

    const json::Token& Configuration::begin() const
    {
      return data().begin ;
    }

    const json::Token& Configuration::end() const
    {
      return data().end ;
    }
    
    bool Configuration::isInitialized() const
    {
      return data().init ;
    }

    void Configuration::initialize( const char* path, unsigned channel )
    {
      std::string   file   ;
      std::ifstream stream ;
      
      data().bus.setChannel( channel ) ;

      stream.open( path ) ;
      data().parser.clear() ;
      if( stream )
      {
        data().init = true ;
        
        // Copy stream's contents into string.
        stream.seekg  ( 0, std::ios::end  ) ;
        file  .reserve( stream.tellg()    ) ;
        stream.seekg  ( 0, std::ios::beg  ) ;

        file.assign ( ( std::istreambuf_iterator<char>( stream ) ), std::istreambuf_iterator<char>() ) ;

        // Feed data to parser.
        data().parser.initialize( file.c_str() ) ;
        data().begin = data().parser.begin() ;
        data().end   = data().parser.end  () ;

        stream.close() ;

        // Send parsed data over event bus.
        for( auto token = data().begin; token != data().end; ++token )
        {
          if( token.isArray() )
          {
            for( unsigned index = 0; index < token.size(); index++ )
            {
              // TODO- Look for a better way than just emitting over all types.
              data().bus.emitIndexed( token.string ( index ), index, token.key() ) ;
              data().bus.emitIndexed( token.number ( index ), index, token.key() ) ;
              data().bus.emitIndexed( token.boolean( index ), index, token.key() ) ;
              data().bus.emitIndexed( token.decimal( index ), index, token.key() ) ;
              data().bus.emit( token.string ( index ), token.key() ) ;
              data().bus.emit( token.number ( index ), token.key() ) ;
              data().bus.emit( token.boolean( index ), token.key() ) ;
              data().bus.emit( token.decimal( index ), token.key() ) ;
            }
          }
          else
          {
            data().bus.emit( token.string (), token.key() ) ;
            data().bus.emit( token.number (), token.key() ) ;
            data().bus.emit( token.boolean(), token.key() ) ;
            data().bus.emit( token.decimal(), token.key() ) ;
          }
        }
      }
      else
      {
        karma::log::Log::output( karma::log::Log::Level::Warning, "Unable to load configuration file: ", path ) ;
        data().init = false ;
      }
    }

    ConfigurationData& Configuration::data()
    {
      return *this->config_data ;
    }

    const ConfigurationData& Configuration::data() const
    {
      return *this->config_data ;
    }
  }
}
