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

/* 
 * File:   TestModule.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 5, 2021, 3:38 PM
 */

#include "TestModule.h"
#include <data/Bus.h>
#include <string>
#include <float.h>
#include <algorithm>
#include <cmath>
#include <iostream>

const unsigned VERSION = 1 ;

namespace iris
{
  bool floatEquals( float v1, float v2 )
  {
    return std::fabs( v1 - v2 ) < FLT_EPSILON ;
  }

  struct TestModuleData
  {
    iris::Bus  bus    ;
    float       thing1 ;
    std::string thing2 ;
    unsigned    thing3 ;

    void setThing1( float val ) ;
    
    void setThing2( const char* val ) ;
    
    void setThing3( unsigned val ) ;
  };
  
  void TestModuleData::setThing1( float val )
  {
    this->thing1 = val ;
  }
  
  void TestModuleData::setThing2( const char* val ) 
  {
    this->thing2 = val ;
  }
    
  void TestModuleData::setThing3( unsigned val )
  {
    this->thing3 = val ;
  }
  
  TestModule::TestModule()
  {
    this->module_data = new TestModuleData() ;
  }

  TestModule::~TestModule()
  {
    delete this->module_data ;
  }

  void TestModule::initialize()
  {
    std::cout << "Initializing Test Module..." << std::endl ;
    if( !floatEquals( data().thing1, 0.25 ) ){  std::cout << "!! Float value failed to set.  !!" << " : " << data().thing1 << std::endl ; exit( 1 ) ; }
    if( data().thing2 != "thing2"           ){  std::cout << "!! String value failed to set. !!" << " : " << data().thing2 << std::endl ; exit( 1 ) ; }
    if( data().thing3 != 2503               ){  std::cout << "!! Integer value failed to set.!!" << " : " << data().thing3 << std::endl ; exit( 1 ) ; }
  }

  void TestModule::subscribe( unsigned id )
  {
    data().bus.setChannel( id ) ;
    
    data().bus.enroll( this->module_data, &TestModuleData::setThing1, iris::OPTIONAL, this->name(), "::thing1" ) ;
    data().bus.enroll( this->module_data, &TestModuleData::setThing2, iris::OPTIONAL, this->name(), "::thing2" ) ;
    data().bus.enroll( this->module_data, &TestModuleData::setThing3, iris::OPTIONAL, this->name(), "::thing3" ) ;
  }

  void TestModule::shutdown()
  {
  
  }

  void TestModule::execute()
  {
    
  }

  TestModuleData& TestModule::data()
  {
    return *this->module_data ;
  }

  const TestModuleData& TestModule::data() const
  {
    return *this->module_data ;
  }
}

/** Exported function to retrive the name of this module type.
 * @return The name of this object's type.
 */
exported_function const char* name()
{
  return "TestModule" ;
}

/** Exported function to retrieve the version of this module.
 * @return The version of this module.
 */
exported_function unsigned version()
{
  return VERSION ;
}

/** Exported function to make one instance of this module.
 * @return A single instance of this module.
 */
exported_function ::iris::Module* make()
{
  return new ::iris::TestModule() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( iris::Module* module )
{
  iris::TestModule* mod ;
  
  mod = dynamic_cast<::iris::TestModule*>( module ) ;
  delete mod ;
}

