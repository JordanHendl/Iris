#include "Log.h"
#include <Athena/Manager.h>
#include <iostream>

using namespace iris::log ;

static athena::Manager manager ;

bool testLog()
{
  for( unsigned i = 0; i < 5; i++ )
  {
    Log::output( Log::Level::Warning, "Whoa cool log message!", i ) ;
  }
  
  return true ;
}

bool testNoWarning()
{
  Log::output( "Message without a level!\n" ) ;
  
  return true ;
}

int main() 
{
  std::cout << "\n-- Testing log library." << std::endl ;
  
  Log::initialize( "./", false ) ;
  Log::setMode( Log::Mode::Verbose ) ;

  manager.initialize( "Iris Logging Library" ) ;
  manager.add( "Log Test"         , &testLog       ) ;
  manager.add( "Log No Level Test", &testNoWarning ) ;
  
  Log::flush() ;
  return manager.test( athena::Output::Verbose ) ;
}

