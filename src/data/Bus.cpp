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
    
    using SubscriberIterator = std::multimap<unsigned, Signal::Subscriber*>::iterator ;
    using PublisherIterator  = std::multimap<unsigned, Signal::Publisher *>::iterator ;
    
    class Subscriber
    {
      public: 
        Subscriber() ;
        Subscriber( const Subscriber& sub ) ;
        ~Subscriber() ;
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
  
  using SignalMap        =  std::multimap<std::string, Signal*                                                      > ;
  using LocalSubscribers =  std::map<std::string, std::pair<Signal*, std::map<unsigned, Signal::SubscriberIterator>>> ;
  using LocalPublishers  =  std::map<std::string, std::pair<Signal*, std::map<unsigned, Signal::PublisherIterator >>> ;
  
  static SignalMap  signal_map ;
  static std::mutex map_lock   ;

  struct BusData
  {
    LocalSubscribers sub_map          ;
    LocalSubscribers required_sub_map ;
    LocalPublishers  pub_map          ;
    unsigned         identifier       ;
    std::mutex       lock             ;
    
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
  
  Signal::Subscriber::~Subscriber()
  {
    this->subscriber_ptr = nullptr ;
  }

  void Signal::Subscriber::signal()
  {
    std::scoped_lock<std::mutex> lock( this->mutex ) ;
    this->is_signaled = true ;
    this->cv.notify_one() ;
  }
  
  void Signal::Subscriber::wait()
  {
    std::unique_lock<std::mutex> lock( this->mutex ) ;
    this->cv.wait( lock, [=] { return this->is_signaled.load() ; } ) ;
    this->reset() ;
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
    iter->second->reset () ;
    delete iter->second ;
    iter->second = nullptr ;
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
        for( auto& iter2 : iter.second.second )
        {
          delete iter2.second->second ;
          signal->second->subscribers.erase( iter2.second ) ;
        }
      }
    }
    
//    if( this->required_sub_map.size() != 0 )
//    {
//      for( auto& iter : this->required_sub_map )
//      {  
//        auto signal = signal_map.find( iter.first ) ;
//        for( auto& iter2 : iter.second.second )
//        {
//          delete iter2.second->second ;
//          signal->second->subscribers.erase( iter2.second ) ;
//        }
//      }
//    }
    
    if( this->pub_map.size() != 0 )
    {
      for( auto& iter : this->pub_map )
      {  
        auto signal = signal_map.find( iter.first ) ;
        for( auto& iter2 : iter.second.second )
        {
          delete iter2.second->second ;
          signal->second->publishers.erase( iter2.second ) ;
        }
      }
    }
    
    this->pub_map         .clear() ;
    this->sub_map         .clear() ;
    this->required_sub_map.clear() ;
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
    data().lock.lock() ;
    for( auto pub : data().pub_map )
    {
      for( auto& pair : pub.second.second )
      {
        auto map = pub.second ;
        auto val = pair.second->second->execute( idx ) ;
        
        for( auto iter = pub.second.first->subscribers.lower_bound( this->UNIVERSAL_TYPE ); iter != pub.second.first->subscribers.upper_bound( this->UNIVERSAL_TYPE ); ++iter )
        {
          iter->second->subscriber().execute( val, idx ) ;
          iter->second->signal() ;
        }
        
        for( auto iter = pub.second.first->subscribers.lower_bound( pair.second->first ); iter != pub.second.first->subscribers.upper_bound( pair.second->first ); ++iter )
        {
          if( pair.second->first != this->UNIVERSAL_TYPE )
          {
            iter->second->subscriber().execute( val, idx ) ;
            iter->second->signal() ;
          }
        }
      }
    }
    data().lock.unlock() ;
  }
  
  void Bus::wait()
  {
    data().lock.lock() ;
    for( auto &signal : data().required_sub_map )
    {
      signal.second.first->signal_mutex.lock() ;
      for( auto &sig : signal.second.second )
      {
        sig.second->second->wait() ;
      }
      signal.second.first->signal_mutex.unlock() ;
    }
    data().lock.unlock() ;
  }
  
  void Bus::clearSubscriptions()
  {
    map_lock.lock() ;
    if( data().sub_map.size() != 0 )
    {
      for( auto& iter : data().sub_map )
      {  
        auto signal = signal_map.find( iter.first ) ;
        for( auto& iter2 : iter.second.second )
        {
          delete iter2.second->second ;
          signal->second->subscribers.erase( iter2.second ) ;
        }
      }
    }

    data().sub_map         .clear() ;
    data().required_sub_map.clear() ;
    map_lock.unlock() ;
  }
  
  void Bus::reset()
  {
    map_lock.lock() ;
    if( data().sub_map.size() != 0 )
    {
      for( auto& iter : data().sub_map )
      {  
        auto signal = signal_map.find( iter.first ) ;
        for( auto& iter2 : iter.second.second )
        {
          delete iter2.second->second ;
          signal->second->subscribers.erase( iter2.second ) ;
        }
      }
    }
    
    if( data().pub_map.size() != 0 )
    {
      for( auto& iter : data().pub_map )
      {  
        auto signal = signal_map.find( iter.first ) ;
        for( auto& iter2 : iter.second.second )
        {
          delete iter2.second->second ;
          signal->second->publishers.erase( iter2.second ) ;
        }
      }
    }

    data().pub_map         .clear() ;
    data().sub_map         .clear() ;
    data().required_sub_map.clear() ;
    map_lock.unlock() ;
  }
  void Bus::enrollBase( const Key& key, Publisher* publisher, unsigned type_id )
  {
    Signal::PublisherIterator pub_iter ;
    
    data().lock.lock() ;
    map_lock.lock() ;
    
    auto iter = signal_map.find( key.str() )      ;
    auto iter2 = data().pub_map.find( key.str() ) ;

    
    if( iter2 != data().pub_map.end() )
    {
      auto type_iter = iter2->second.second.find( type_id ) ;
      if( type_iter != iter2->second.second.end() )
      {
        iter->second->remove( type_iter->second ) ;
        data().pub_map.erase( key.str() ) ;
      }
    }
    else
    {
      data().pub_map[ key.str() ] ;
    }

    if( iter == signal_map.end() )
    {
      iter = signal_map.insert( { std::string( key.str() ), new Signal() } ) ;
      
      pub_iter = iter->second->insert( type_id, publisher ) ;
    }
    else
    {
      pub_iter = iter->second->insert( type_id, publisher ) ;
    }
    
    
    data().pub_map[ key.str() ].first = iter->second                   ;
    data().pub_map[ key.str() ].second.insert( { type_id, pub_iter } ) ;

    map_lock.unlock() ;
    data().lock.unlock() ;
  }
  
  void Bus::enrollBase( const Key& key, Subscriber* subscriber, Requirement required, unsigned type_id )
  {
    Signal::SubscriberIterator sub_iter ;
    
    data().lock.lock() ;
    map_lock.lock() ;
    
    auto iter  = signal_map.find( key.str() )     ;
    auto iter2 = data().sub_map.find( key.str() ) ;
    
    if( iter2 != data().sub_map.end() )
    {
      auto type_iter = iter2->second.second.find( type_id ) ;

      if( type_iter != iter2->second.second.end() )
      {
        iter->second->remove( type_iter->second ) ;
        data().sub_map.erase( iter2 ) ;
        if( data().required_sub_map.find( key.str() ) != data().required_sub_map.end() )
        {
          data().required_sub_map.erase( key.str() ) ;
        }
      }
    }

    if( iter == signal_map.end() )
    {
      iter = signal_map.insert( { std::string( key.str() ), new Signal() } ) ;
      
      sub_iter = iter->second->insert( type_id, subscriber ) ;
    }
    else
    {
      sub_iter = iter->second->insert( type_id, subscriber ) ;
    }
    
    data().sub_map[ key.str() ].first = iter->second                   ;
    data().sub_map[ key.str() ].second.insert( { type_id, sub_iter } ) ;
    
    if( required == iris::REQUIRED )
    {
      data().required_sub_map[ key.str() ].first = iter->second                   ;
      data().required_sub_map[ key.str() ].second.insert( { type_id, sub_iter } ) ;
    }

    map_lock.unlock() ;
    data().lock.unlock() ;
  }
  
  void Bus::emitBase( const Key& key, const void* value, unsigned type_id, unsigned idx )
  {
    auto iter = signal_map.find( key.str() ) ;
    
    data().lock.lock() ;
    if( iter != signal_map.end() )
    {
      for( auto sub = iter->second->subscribers.lower_bound( type_id ); sub != iter->second->subscribers.upper_bound( type_id ); ++sub )
      {
        sub->second->subscriber().execute( value, idx ) ;
        sub->second->signal() ;
      }
    }
    data().lock.unlock() ;
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