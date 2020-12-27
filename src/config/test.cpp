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

  return 1 ;
}