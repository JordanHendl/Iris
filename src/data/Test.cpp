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
  bus.emit() ;
  std::this_thread::sleep_for( std::chrono::seconds( 5 ) ) ;
  bus.emit( TEST_VALUE_2, "obj_test_2" ) ;
}

int main( int argc, char** argv ) 
{
  TestObject obj ;
  
  bus.enroll ( &setter, "text"                                   ) ;
  bus.enroll ( &obj   , &TestObject::setter, "obj_test"          ) ;
  bus.publish( &getter, "text"                                   ) ;
  bus.publish( &obj   , &TestObject::getter, "obj_test"          ) ;

  f = TEST_VALUE ;
  std::cout << " Waiting on inputs.. " << std::endl ;
  bus.emit() ;
  bus.wait() ;
  std::cout << " Finished. " << std::endl ;
  
  return 0;
}

