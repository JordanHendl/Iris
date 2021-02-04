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
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <utility>
#include <condition_variable>
#include <mutex>
#include <atomic>

namespace iris
{ 
  void operator<<( Key& first, const char* second )
  {
    std::stringstream stream ;
    std::string       string ;

    stream << first.str()  ;
    stream << second       ;
    
    string = stream.str() ;
    first.setStr( string.c_str() ) ;
  }

  void operator<<( Key& first, unsigned second )
  {
    std::stringstream stream ;
    std::string       string ;

    stream << first.str()  ;
    stream << second       ;
    
    string = stream.str() ;
    first.setStr( string.c_str() ) ;
  }
  
  void operator<<( Key& first, float second )
  {
    std::stringstream stream ;
    std::string       string ;
    
    stream << first.str() ;
    stream << second      ;
    
    string = stream.str() ;
    first.setStr( string.c_str() ) ;
  }
  
  void operator<<( Key& first, const Key& second )
  {
    std::stringstream stream ;
    std::string       string ;
    
    stream << first.str()  ;
    stream << second.str() ;
    
    string = stream.str() ;
    first.setStr( string.c_str() ) ;
  }

  struct KeyData
  {
    std::string str ;
  };
  
  struct Signal 
  {
    class Subscriber ;
    class Publisher  ;
    
    typedef std::multimap<unsigned, Signal::Subscriber*>::iterator SubscriberIterator ;
    typedef std::multimap<unsigned, Signal::Publisher *>::iterator PublisherIterator  ;
    
    class Subscriber
    {
      public: 
        Subscriber() ;
        Subscriber( const Subscriber& sub ) ;
        void initialize( Bus::Subscriber* sub ) ;
        void signal() ;
        void wait() ;
        Bus::Subscriber& subscriber() ;
        void reset() ;
      private:
        std::condition_variable  cv             ;
        std::mutex               mutex          ;
        Bus::Subscriber*         subscriber_ptr ;
        std::atomic<bool>        is_signaled    ;
    };
    
    class Publisher
    {
      public: 
        Publisher() ;
        Publisher( const Publisher& sub ) ;
        void initialize( Bus::Publisher* sub ) ;
        void signal() ;
        void wait() ;
        const void* execute( unsigned idx ) ;
        void reset() ;
      private:
        Bus::Publisher* pub_ptr ;
        
    };
    
    /**
     * @param id
     * @param sub
     * @return 
     */
    SubscriberIterator insert( unsigned id, Bus::Subscriber* sub ) ;
    
    /**
     * @param id
     * @param pub
     * @return 
     */
    PublisherIterator  insert( unsigned id, Bus::Publisher*  pub ) ;
    
    /**
     * @param iter
     */
    void remove( SubscriberIterator& iter ) ;
    
    /**
     * @param iter
     */
    void remove( PublisherIterator& iter ) ;
    
    std::multimap<unsigned, Signal::Subscriber*> subscribers  ;
    std::multimap<unsigned, Signal::Publisher *> publishers   ;
    std::mutex                                   signal_mutex ;
  };
  
  typedef std::multimap<std::string, Signal*                                       > SignalMap        ;
  typedef std::multimap<std::string, std::pair<Signal*, Signal::SubscriberIterator>> LocalSubscribers ;
  typedef std::multimap<unsigned   , std::pair<Signal*, Signal::PublisherIterator >> LocalPublishers  ;
  
  static SignalMap  signal_map ;
  static std::mutex map_lock   ;

  struct BusData
  {
    LocalSubscribers sub_map          ;
    LocalSubscribers required_sub_map ;
    LocalPublishers  pub_map          ;
    unsigned         identifier       ;
    
    BusData() ;
    BusData& operator=( const BusData& bus ) ;
    ~BusData() ;
  };

  Key::Key()
  {
    this->key_data = new KeyData() ;
  }

  Key::Key( const Key& string )
  {
    this->key_data = new KeyData() ;
    *this->key_data = *string.key_data ;
  }
  
  void Key::operator=( const Key& string )
  {
    *this->key_data = *string.key_data ;
  }
      
  Key::~Key()
  {
    delete this->key_data ;
  }
  
  void Key::setStr( const char* str )
  {
    data().str = str ;
  }

  const char* Key::str() const
  {
    return data().str.c_str() ;
  }

  KeyData& Key::data()
  {
    return *this->key_data ;
  }

  const KeyData& Key::data() const
  {
    return *this->key_data ;
  }

  Signal::Subscriber::Subscriber()
  {
    this->subscriber_ptr = nullptr ;
    this->is_signaled = false ;
  }
  
  Signal::Subscriber::Subscriber( const Signal::Subscriber& cont )
  {
    this->subscriber_ptr = cont.subscriber_ptr ;
    this->is_signaled.exchange( cont.is_signaled.load() ) ;
  }
  
  void Signal::Subscriber::signal()
  {
    this->is_signaled = true ;
    this->cv.notify_one() ;
  }
  
  void Signal::Subscriber::wait()
  {
    std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>( this->mutex ) ;
    this->cv.wait( lock, [=] { return this->is_signaled.load() ; } ) ;
    this->reset() ;
    lock.unlock() ;
  }
  
  void Signal::Subscriber::initialize( Bus::Subscriber* sub )
  {
    this->subscriber_ptr = sub ;
  }

  Bus::Subscriber& Signal::Subscriber::subscriber()
  {
    return *this->subscriber_ptr ;
  }
  
  void Signal::Subscriber::reset()
  {
    this->is_signaled = false ;
  }
  
  Signal::Publisher::Publisher()
  {
    this->pub_ptr = nullptr ;
  }
  
  void Signal::Publisher::initialize( Bus::Publisher* pub )
  {
    this->pub_ptr = pub ; 
  }
  
  Signal::SubscriberIterator Signal::insert( unsigned id, Bus::Subscriber* sub )
  {
    Signal::Subscriber* signal_sub = new Signal::Subscriber() ;
    signal_sub->initialize( sub ) ;
    
    this->signal_mutex.lock() ;
    auto ret = this->subscribers.insert( { id, signal_sub } ) ;
    this->signal_mutex.unlock() ;
    return ret ;
  }
  
  Signal::PublisherIterator Signal::insert( unsigned id, Bus::Publisher* pub )
  {
    Signal::Publisher* signal_pub = new Signal::Publisher() ;
    signal_pub->initialize( pub ) ;
    
    this->signal_mutex.lock() ;
    auto ret = this->publishers.insert( { id, signal_pub } ) ;
    this->signal_mutex.unlock() ;
    return ret ;
  }
  
  void Signal::remove( Signal::SubscriberIterator& iter )
  {
    this->signal_mutex.lock() ;
    this->subscribers.erase( iter ) ;
    this->signal_mutex.unlock() ;
  }
  
  void Signal::remove( Signal::PublisherIterator& iter )
  {
    this->signal_mutex.lock() ;
    this->publishers.erase( iter ) ;
    this->signal_mutex.unlock() ;
  }
  
  const void* Signal::Publisher::execute( unsigned idx )
  {
    return this->pub_ptr->publish( idx ) ;
  }
  
  BusData& BusData::operator=( const BusData& bus )
  {
    this->identifier = bus.identifier ;
    this->pub_map    = bus.pub_map    ;
    this->sub_map    = bus.sub_map    ;
    
    return *this ;
  }

  BusData::BusData()
  {
    this->identifier = 0 ;
  }
  
  BusData::~BusData()
  {
    map_lock.lock() ;
    if( this->sub_map.size() != 0 )
    {
      for( auto& iter : this->sub_map )
      {
        auto signal = signal_map.find( iter.first ) ;
        
        delete( iter.second.second->second ) ;
        signal->second->subscribers.erase( iter.second.second ) ;
      }
    }
    map_lock.unlock() ;
  }
  
  Bus& Bus::operator =( const Bus& bus )
  {
    *this->bus_data = *bus.bus_data ;
    return *this ;
  }
  
  Bus::Bus( unsigned id )
  {
    this->bus_data = new BusData() ;
    setChannel( id ) ;
  }
  
  Bus::Bus( const Bus& bus )
  {
    this->bus_data = new BusData() ;
    *this->bus_data = *bus.bus_data ;
  }
  
  Bus::~Bus()
  {
    delete this->bus_data ;
  }
  
  BusData& Bus::data()
  {
    return *this->bus_data ;
  }
  
  const BusData& Bus::data() const
  {
    return *this->bus_data ;
  }
  
  void Bus::emit( unsigned idx )
  {
    for( auto pub : data().pub_map )
    {
      auto map = pub.second ;
      auto val = map.second->second->execute( idx ) ;
      
      for( auto iter = pub.second.first->subscribers.lower_bound( pub.first ); iter != pub.second.first->subscribers.upper_bound( pub.first ); ++iter )
      {
        iter->second->subscriber().execute( val, idx ) ;
        iter->second->signal() ;
      }
    }
  }
  
  void Bus::wait()
  {
    for( auto &signal : data().required_sub_map )
    {
      signal.second.second->second->wait() ;
    }
  }
  
  void Bus::enrollBase( const Key& key, Publisher* publisher, unsigned type_id )
  {
    Signal::PublisherIterator pub_iter ;
    
    map_lock.lock() ;
    
    auto iter = signal_map.find( key.str() ) ;
    
    if( iter == signal_map.end() )
    {
      iter = signal_map.insert( { std::string( key.str() ), new Signal() } ) ;
      
      pub_iter = iter->second->insert( type_id, publisher ) ;
    }
    else
    {
      pub_iter = iter->second->insert( type_id, publisher ) ;
    }
    
    data().pub_map.insert( { type_id , std::make_pair( iter->second, pub_iter ) } ) ;
    
    map_lock.unlock() ;
  }
  
  void Bus::enrollBase( const Key& key, Subscriber* subscriber, bool required, unsigned type_id )
  {
    Signal::SubscriberIterator sub_iter ;
    
    map_lock.lock() ;
    
    auto iter = signal_map.find( key.str() ) ;
    
    if( iter == signal_map.end() )
    {
      iter = signal_map.insert( { std::string( key.str() ), new Signal() } ) ;
      
      sub_iter = iter->second->insert( type_id, subscriber ) ;
    }
    else
    {
      sub_iter = iter->second->insert( type_id, subscriber ) ;
    }
    
    data().sub_map.insert( { std::string( key.str() ), std::make_pair( iter->second, sub_iter ) } ) ;
    
    if( required )
    {
      data().required_sub_map.insert( { std::string( key.str() ), std::make_pair( iter->second, sub_iter ) } ) ;
    }

    map_lock.unlock() ;
  }
  
  void Bus::emitBase( const Key& key, const void* value, unsigned type_id, unsigned idx )
  {
    auto iter = signal_map.find( key.str() ) ;
    
    if( iter != signal_map.end() )
    {
      for( auto sub = iter->second->subscribers.lower_bound( type_id ); sub != iter->second->subscribers.upper_bound( type_id ); ++sub )
      {
        sub->second->subscriber().execute( value, idx ) ;
        sub->second->signal() ;
      }
    }
  }
  
  unsigned Bus::id()
  {
    return data().identifier ;
  }

  void Bus::setChannel( unsigned id )
  {
    data().identifier = id ;
  }
}

