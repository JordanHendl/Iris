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

#pragma once

namespace iris
{
  /** Class for timing events in the program.
   */
  class Timer
  {
    public:
      
      /** Default constructor. Initializes implementation.
       */
      Timer() ;
      
      /** Default Deconstructor. Releases implementation.
       */
      ~Timer() ;
      
      /** Method to initialize this timer.
       * @param name The name to associate with this timer and it's output.
       */
      void initialize( const char* name = "" ) ;
      
      /** Method to check whether or not this object is initialized.
       * @return Whether or not thie object is initialized.
       */
      bool isInitialized() const ;
      
      /** Method to start this timer.
       */
      void start() ;
      
      /** Method to toggle pausing of this timer.
       */
      void pause() ;
      
      /** Method to stop this timer.
       */
      void stop() ;
      
      /** Method to recieve the calculated time after a call to start & stop.
       * @return The amount of time elapsed, in milliseconds.
       */
      double time() const ;
      
      /** Method to print this object's calculated time to stdout.
       */
      void print() const ;
      
      /** Method to retrieve the output string of this object. This is what gets outputted in @print.
       * @return The string representation of this object's output.
       */
      const char* output() const ;
    private:
      
      /** Forward declared pointer to this object's underlying data.
       */
      struct TimerData* timer_data ;
      
      /** Method to retrieve reference to this object's underlying data.
       * @return Reference to this object's underlying data.
       */
      TimerData& data() ;
      
      /** Method to retrieve reference to this object's underlying data.
       * @return Reference to this object's underlying data.
       */
      const TimerData& data() const ;
  };
}

