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

#include "Timer.h"
#include <Athena/Manager.h>
#include <chrono>
#include <thread>
#include <iostream>

static athena::Manager manager ;

athena::Result testProfiler()
{
  iris::Timer timer ;
  
  timer.start() ;
  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) ) ;
  timer.stop() ;
  
  if( timer.time() > 1000.f * 999.f ) 
  {
    return athena::Result::Pass ;
  }
  
  std::cout << "Time was: " << timer.time() << "μs. Expected: " << 1000.f * 999.f << "μs" << std::endl ;
  return athena::Result::Fail ;
}

int main()
{
  manager.initialize( "Iris Profiler Test" ) ;
  manager.add( "Expected Time", &testProfiler ) ;
  return manager.test( athena::Output::Verbose ) ; 
}