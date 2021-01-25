/*
 * Copyright (C) 2021 Jordan Hendl
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
 * File:   Test.cpp
 * Author: Jordan Hendl
 *
 * Created on January 15, 2021, 2:23 AM
 */

#include <Athena/Manager.h>
#include <iostream>

static athena::Manager manager ;

class noimpltest
{
  public:
    noimpltest() = default ;
    ~noimpltest() = default ;
    
    void assign( unsigned id )
    {
      this->data = id ;
    }

    unsigned get() const
    {
      return this->data ;
    }
  private:
    unsigned data = 1 ;
};

struct impltestdata
{
  unsigned data = 1 ;
};

class impltest
{
  public:
    impltest()
    {
      this->impl_data = new impltestdata() ;
    };
    
    ~impltest()
    {
      delete this->impl_data ;
    };
    
    void assign( unsigned id )
    {
      data().data = id ;
    }
    
    unsigned get() const
    {
      return data().data ;
    }
    
  private:
    struct impltestdata* impl_data ;
    impltestdata& data()
    {
      return *this->impl_data ;
    };
    const impltestdata& data() const
    {
      return *this->impl_data ;
    };
};


bool testNoImpl()
{
  noimpltest test ;
  volatile unsigned   num  ;
  for( unsigned i = 0; i < 10000; i++ )
  {
    test.assign( i ) ;
    num = test.get() ;
    
    std::cout << num ;
  }
  
  return num ;
}

bool testImpl()
{
  impltest test ;
  volatile unsigned num  ;
  for( unsigned i = 0; i < 10000; i++ )
  {
    test.assign( i ) ;
    num = test.get() ;
    
    std::cout << num ;
  }
  
  return num ;
}

int main()
{
  manager.add( "1) No PIMPL speed test", &testNoImpl ) ;
  manager.add( "2) PIMPL speed test"   , &testImpl   ) ;

  return manager.test( athena::Output::Verbose ) ; 
//  return 0 ;
}

