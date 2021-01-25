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
#include <Athena/Manager.h>
#include <cmath>
#include <iostream>

static const float          TEST_VALUE   = 0.052005f       ;
static const float          TEST_VALUE_2 = 0.254565f       ;
static const float          TEST_VALUE_3 = 0.612335f       ;
static const unsigned       TEST_ARR[] = { 5, 4, 3, 2, 1 } ;
static iris::Bus            bus                            ;
static athena::Manager      manager                        ;
static float                v                              ;
static float                f                              ;
static unsigned             index[] = { 0, 0, 0, 0, 0 }    ;

bool equals( float v1, float v2 )
{
  return std::fabs( v1 - v2 ) < FLT_EPSILON ;
}

struct TestObject
{
  float output       ;
  float manual_input ;
  float input        ;
  
  TestObject()
  {
    output = TEST_VALUE ;
  }

  void setter( float val )
  {
    input = val ;
  }
  
  void manual_setter( float val )
  {
    this->manual_input = val ;
  }
  
  float getter()
  {
    return output ;
  }
  
  bool checkMethodSetter()
  {
    iris::Bus bus ;
    bus.enroll ( this, &TestObject::setter, true, "obj_test"   ) ; // Method Setter.
    bus.publish( this, &TestObject::getter, "obj_test"   ) ; // Method Getter.
    bus.emit() ;
    
    if( equals( this->input, TEST_VALUE ) )  
    {
      return true ;
    }
    
    return false ;
  }
  
  bool checkManualSetter()
  {
    iris::Bus bus ;
    bus.enroll ( this, &TestObject::manual_setter, true, "manual_set" ) ; // Manual Setter.
    bus.emit( TEST_VALUE_3, "manual_set" ) ; // Manually send data.

    if( equals( this->manual_input, TEST_VALUE_3 ) )
    {
      return true ;
    }
    
    return false ;
  }
};
void setter( float val )
{
  v = val ;
}

float getter()
{
  return f ;
}
void indexedSetter( unsigned idx, unsigned val )
{
  index[ idx ] = val ;
}

unsigned indexedGetter( unsigned idx )
{
  return TEST_ARR[ idx ] ;
}

bool testIndexedSetter()
{
  iris::Bus bus ;
  bus.enroll ( &indexedSetter, true, "indexed" ) ; // Indexed Setter.
  bus.publish( &indexedGetter, "indexed" ) ; // Indexed Getter.
  
  for( unsigned i = 0; i < 5; i++ )
  {
    bus.emit( i ) ;
    if( TEST_ARR[ i ] != index[ i ] )
    {
      return false ;
    }
  }
  return true ;
}
static iris::Bus speed_bus ;

template<unsigned i>
void setSpeed( unsigned val )
{
  val = val ;
}

template<unsigned i>
unsigned getSpeed()
{
  return i ;
}

bool testEmitSpeed()
{
  speed_bus.emit() ;
  
  return true ;
}

bool testFunctionSetter()
{
  iris::Bus bus ;
  bus.enroll ( &setter, true, "test_1" ) ; // Function Setter.
  bus.publish( &getter, "test_1" ) ; // Function Getter.
  
  bus.emit() ;
  if( equals( v, TEST_VALUE_2 ) )
  {
    return true ;
  }
  
  return false ;
}

int main() 
{ 
  TestObject obj ;
  f = TEST_VALUE_2 ;
  
  for( unsigned i = 0; i < 100; i++ )
  {
    speed_bus.enroll ( &setSpeed<0>, true,"speed" ) ;
    speed_bus.publish( &getSpeed<0>,"speed" ) ;
  }
  manager.add( "Function Test"      , &testFunctionSetter                  ) ;
  manager.add( "Method Test"        , &obj, &TestObject::checkMethodSetter ) ;
  manager.add( "Manual Test"        , &obj, &TestObject::checkManualSetter ) ;
  manager.add( "Indexed Test"       , &testIndexedSetter                   ) ;
  manager.add( "100 Emit Speed Test", &testEmitSpeed                       ) ;
  
  std::cout << "\n-- Testing Iris Data Bus" << std::endl ;
  return manager.test( athena::Output::Verbose ) ;
}

