#include "Log.h"

int main() 
{
  using namespace karma::log ;
  
  Log::initialize( "/var/tmp/" ) ;
  
  for( unsigned i = 0; i < 200; i++ )
  {
    Log::output( Log::Level::Warning, "Whoa cool log message!", i ) ;
  }
    
  Log::output( " This is a log message with no log level attached! " ) ;
  
  Log::write() ;
  return 0;
}

