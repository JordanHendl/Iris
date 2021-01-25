#include "Driver.h"

int main( int argc, const char* argv[] )
{
  IrisDriver app ;
  
  app.initialize( argc, argv ) ;
  
  return app.run() ;
}