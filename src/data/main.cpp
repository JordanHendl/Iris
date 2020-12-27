#include "Bus.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <assert.h>
#include <float.h>
#include <algorithm>
#include <cmath>
#include <chrono>

static const float TEST_VALUE   = 0.052005f ;
static const float TEST_VALUE_2 = 0.254565f ;
static       ::karma::Bus bus               ;
static float v                              ;
static float f                              ;

bool equals( float v1, float v2 )
{
  return std::fabs( v1 - v2 ) < FLT_EPSILON ;
}
struct TestObject
{
  float output ;
  float input  ;
  
  TestObject()
  {
    output = TEST_VALUE ;
  }
  void setter( float val )
  {
    std::cout << " Method Set " << std::endl ;
    input = val ;
    assert( equals( input, TEST_VALUE ) ) ;
  }
  
  void manual_setter( float val )
  {
    std::cout << " Manual Method Set " << std::endl ;
    assert( equals( val, TEST_VALUE_2 ) ) ;
  }
  
  float getter()
  {
    return output ;
  }
};
void setter( float val )
{
  std::cout << " Function Set " << std::endl ;
  v = val ;
  assert( equals( v, TEST_VALUE ) ) ;
}

float getter()
{
  assert( equals( f, TEST_VALUE ) ) ;
  return f ;
}

void threadFunction()
{
  f = TEST_VALUE ;
  bus.emit() ;
  std::this_thread::sleep_for( std::chrono::seconds( 5 ) ) ;
  bus.emit( TEST_VALUE_2, "obj_test_2" ) ;
}

int main( int argc, char** argv ) 
{
  TestObject obj ;
  
  bus.enroll ( &setter, "text"                                   ) ;
  bus.enroll ( &obj   , &TestObject::setter, "obj_test"          ) ;
  bus.enroll ( &obj   , &TestObject::manual_setter, "obj_test_2" ) ;
  bus.publish( &getter, "text"                                   ) ;
  bus.publish( &obj   , &TestObject::getter, "obj_test"          ) ;

  std::thread thread( &threadFunction ) ;
  
  std::cout << " Waiting on inputs.. " << std::endl ;
  bus.wait() ;
  std::cout << " Finished. " << std::endl ;
  
  thread.join() ;
  return 0;
}

