#include "Timer.h"
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

namespace karma
{
  namespace ms
  {
    struct TimerData
    {
      typedef std::chrono::time_point<std::chrono::steady_clock> StartTime      ;
      typedef std::chrono::duration  <double                   > CalculatedTime ;
      
      StartTime           current_time ; ///< The snapped off time when the Timer is started.
      CalculatedTime      paused_time  ; ///< The elapsed time of this timer.
      CalculatedTime      elapsed      ; ///< The elapsed time of this timer.
      std::string         name         ; ///< The name of this object. Used when printing output.
      mutable std::string output       ; ///< The holder for the output string of this object.
      bool                init         ; ///< Whether or not the timer is initialized.
      bool                running      ; ///< Whether or not the timer is currently active.
      bool                paused       ; ///< Whether or not this timer is paused.

      /** Default constructor. Initializes this object's data.
       */
      TimerData() ;
    };

    TimerData::TimerData()
    {
      this->init   = false ;
      this->name   = ""    ;
      this->paused = false ;
    }

    Timer::Timer()
    {
      this->timer_data = new TimerData() ;
    }

    Timer::~Timer()
    {
      delete this->timer_data ;
    }
    
    void Timer::initialize( const char* name )
    {
      data().name = name ;
      data().init = true ;
    }
    
    bool Timer::isInitialized() const 
    {
      return data().init ;
    }

    void Timer::start()
    {
      data().current_time = std::chrono::steady_clock::now()              ;
      data().running      = true                                          ;
      data().paused_time  = static_cast<TimerData::CalculatedTime>( 0.0 ) ;
    }
    
    void Timer::pause()
    {
      if( data().running )
      {
        if( data().paused )
        {
          data().current_time = std::chrono::steady_clock::now() ;
          data().paused = false ;
        }
        else
        {
          data().paused_time = data().paused_time + ( std::chrono::steady_clock::now() - data().current_time ) ;
          data().paused = true ;
        }
      }
    }

    void Timer::stop()
    {
      if( data().running )
      {
        data().elapsed = data().paused_time + ( std::chrono::steady_clock::now() - data().current_time ) ;
        data().running = false                                                                           ;
      }
    }

    double Timer::time() const
    {
      auto curr = data().running ? std::chrono::duration_cast<std::chrono::microseconds>( data().paused_time + ( std::chrono::steady_clock::now() - data().current_time ) ) 
                                 : std::chrono::duration_cast<std::chrono::microseconds>( data().elapsed ) ;
      return curr.count() / 1000.f ;
    }

    void Timer::print() const
    {
      auto curr = data().running ? std::chrono::duration_cast<std::chrono::microseconds>( data().paused_time + ( std::chrono::steady_clock::now() - data().current_time ) ) 
                                 : std::chrono::duration_cast<std::chrono::microseconds>( data().elapsed ) ;
      
      std::cout << data().name << " -> " << curr.count() / 1000.f << "\n" ;
    }
    
    const char* Timer::output() const
    {
      const auto curr = data().running ? std::chrono::duration_cast<std::chrono::microseconds>( data().paused_time + ( std::chrono::steady_clock::now() - data().current_time ) ) 
                                 : std::chrono::duration_cast<std::chrono::microseconds>( data().elapsed ) ;
      std::stringstream str ;
      
      str << data().name      ;
      str << " -> "           ;
      str << ( curr.count() / 1000.f ) ;
      
      data().output = str.str().c_str() ;
      return data().output.c_str() ;
    }

    TimerData& Timer::data()
    {
      return *this->timer_data ;
    }

    const TimerData& Timer::data() const
    {
      return *this->timer_data ;
    }
  }
}

