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

#include "Configuration.h"
#include "Parser.h"
#include <iostream>
int main( int argc, const char** argv )
{
  karma::config::Configuration config ;
  karma::config::json::Token   token  ;

  if( argc == 2 )
  {
    config.initialize( argv[1] ) ;

    for( auto token = config.begin(); token != config.end(); ++token )
    {
      if( token.isArray() )
      {
        for( unsigned index = 0; index < token.size(); index++ )
        {
          std::cout << token.key() << "[" << index << "]" << " : " << token.string( index ) << std::endl ;
        }
      }
      else
      {
        std::cout << token.key() << " : " << token.string() << std::endl ;
      }
    }
    return 0 ;
  }
  else
  {
    std::cout << "usage: config_test <file>.json" << std::endl ;
  }

  return 0 ;
}