#ifndef DATA_BUS_H
#define DATA_BUS_H
#include <assert.h>

namespace karma
{
  /** Container for compile-time type info.
   */
  struct TypeInfo
  {
    unsigned    ctti_length ;
    unsigned    ctti_hash   ;
    const char* ctti_name   ;
  };

  /** Wrapper class for generating a key using variadic templates.
   */
  class Key
  {
    public:
      /** Default Constructor.
       */
      Key() ;
      
      /** Copy constructor. Copies input object.
       * @param string The key object to copy.
       */
      Key( const Key& string ) ;
      
      /** Deconstructor.
       */
      ~Key() ;
      
      /** Equals operator. Assigns this object's value to the input's.
       * @param string The object to copy data from.
       */
      void operator=( const Key& string ) ;
      
      /** Method to set this object's internal string.
       * @param str The C-string to use for this object's internal string.
       */
      void setStr( const char* str ) ;
      
      /** Method to retrieve the internal C-string of this object.
       * @return The C-string representation of this object's internal string.
       */
      const char* str() const ;
      
    private:
      struct KeyData* key_data ;
      KeyData& data() ;
      const KeyData& data() const ;
  };

  /** Function to retrieve a compile-time type info object for the input template type.
   * @return Compile-time type information object reference.
   */
  template<typename Type>
  const TypeInfo& typeinfo() ;
  
  /** Operator overloads for converting data types into a Key. 
   * Custom overloads can be made if you wish for easier key generations.
   */
  void operator<<( Key& first, const char* second ) ;
  void operator<<( Key& first, unsigned second ) ;
  void operator<<( Key& first, float second ) ;
  void operator<<( Key& first, const Key& second ) ;
  
  /** Compile-time function to generate a unsigned integer hash from input parameters.
   * @param str The string to hash.
   * @param start The start of the string to hash.
   * @param end The end of the string to hash.
   * @param h The salt of the generated hash.
   * @return The generated unsigned integer hash value.
   */
  constexpr unsigned hash( const char* str, unsigned start, unsigned end, unsigned h = 5381 ) ;
  
  /** Function to find the index of the given substring in the input string.
   * @param str The whole data string to do the look up.
   * @param substr The substring to find in the parent string.
   * @return The starting index of the substring in the parent string. 
   */
  constexpr unsigned find( const char* str, const char* substr ) ;

  /** Class to handle data transfer between modules.
   * @note This object hashes the type information, which can have collisions.
   * 
   *       E.g.  bus[ "output" ].attach<unsigned>     ( &getterFunction  ) ;
   *             bus[ "output" ].attach<CustomObject1>( &getterFunction2 ) ;
   *             bus[ "output" ].attach<CustomObject2>( &getterFunction3 ) ;
   * 
   *   There is a possibility that the types unsigned, CustomObject1 & CustomObject2 have a collision. 
   *   This would cause all 3 functions to recieve all 3 types, if there is a collision. 
   * 
   *   CRC32 hash is used for the typing so the likeliness of a collision is low, but mass subscription to the same key with many different
   *   types may cause problems.
   */
  class Bus
  {
    public:
      
      /** Class for publishing data through function pointers AKA 'getters'.
       */
      class Publisher
      {
        public:
          /** Virtual deconstructor.
           */
          virtual ~Publisher() {} ;
          
          /** Method to publish the object's internal data through it's function pointer.
           * @param index The index to use for publishing.
           * @return The internal function's published data.
           */
          virtual const void* publish( unsigned index = 0 ) = 0 ;
      };
      
      /** Class for recieving published data through function pointers AKA 'setters'.
       */
      class Subscriber
      {
        public:
          /** Virtual deconstructor.
           */
          virtual ~Subscriber() {} ;
          
          /** Method to fullfill a subscription using the input pointer.
           * @param pointer Pointer to the data requested by this object's subscription.
           * @param idx The index to use for the subscription.
           */
          virtual void execute( const void* pointer, unsigned idx = 0 ) = 0 ;
      };
      
      /** Default constructor. Initializes this object's data.
       * @param id The channel to associate with this event bus.
       */
      Bus( unsigned id = 0 ) ;
      
      /**
       * @param bus
       */
      Bus( const Bus& bus ) ;
      
      /**
       * @param bus
       * @return 
       */
      Bus& operator=( const Bus& bus ) ;
      
      /** Deconstructor. Releases this object's data.
       */
      ~Bus() ;
      
      /** Method to wait on all registered actions to occur.
       */
      void wait() ;
      
      /** Method to send functions set to publish out through the Bus.
       */
      void emit( unsigned idx = 0 ) ;
      
      /** Method to manually publish data through the bus to subscriptions.
       * @param value The data to publish.
       * @param idx The index to use for subscriptions.
       * @param args The key of the signal to send the data over.
       */
      template<class Value, typename ... Keys>
      void emitIndexed( const Value& value, unsigned idx, Keys... args ) ;
      
      /** Method to manually publish data through the bus to subscriptions.
       * @param value The data to publish.
       * @param args The key of the signal to send the data over.
       */
      template<class Value, typename ... Keys>
      void emit( const Value& value, Keys... args ) ;
      
      /** Method to enroll a subscription in the bus. 
       *  AKA Set a setter function pointer to receive data copy.
       * @param setter The function pointer of the setter to recieve data.
       * @param args The arguments that make up the name of signal to send the data over.
       */
      template<typename ... Keys, class Value>
      void enroll( void (*setter)( Value ), Keys... args ) ;

      /** Method to enroll a subscription in the bus. 
       *  AKA Set a setter function pointer to receive data via const-reference.
       * @param setter The function pointer of the setter to recieve data.
       * @param args The arguments that make up the name of signal to send the data over.
       */
      template<typename ... Keys, class Value>
      void enroll( void (*setter)( const Value& ), Keys... args ) ;

      /** Method to enroll an indexed subscription in the bus. 
       *  AKA Set a setter function pointer to receive data via value.
       * @param setter The function pointer of the setter to recieve data.
       * @param args The arguments that make up the name of signal to send the data over.
       */      
      template<typename ... Keys, class Value>
      void enroll( void (*setter)( unsigned, Value ), Keys... args ) ;
      
      /** Method to enroll an indexed subscription in the bus. 
       *  AKA Set a setter function pointer to receive data via const-reference.
       * @param setter The function pointer of the setter to recieve data.
       * @param args The arguments that make up the name of signal to send the data over.
       */      
      template<typename ... Keys, class Value>
      void enroll( void (*setter)( unsigned, Value const & ), Keys... args ) ;
      
      /** Method to enroll a method subscription in the bus.
       * @param obj The object to use for calling the subscription.
       * @param setter The function pointer to the setter to recieve data via copy.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void enroll( Object* obj, void (Object::*setter)( Value ), Keys... args ) ;
      
      /** Method to enroll a method subscription in the bus.
       * @param obj The object to use for calling the subscription.
       * @param setter The function pointer to the setter to recieve data via const-reference.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void enroll( Object* obj, void (Object::*setter)( Value const & ), Keys... args ) ;
      
      /** Method to enroll an indexed method subscription in the bus.
       * @param obj The object to use for calling the subscription.
       * @param setter The function pointer to the setter to recieve data via copy.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void enroll( Object* obj, void (Object::*setter)( unsigned, Value ), Keys... args ) ;
      
      /** Method to enroll an indexed method subscription in the bus.
       * @param obj The object to use for calling the subscription.
       * @param setter The function pointer to the setter to recieve data via const-reference.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void enroll( Object* obj, void (Object::*setter)( unsigned, Value const & ), Keys... args ) ;
      
      /** Method to set a publisher in the bus.
       * @param getter The function pointer to use for publishing data via copy.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Value>
      void publish( Value (*getter)(), Keys... args ) ;

      /** Method to set a publisher in the bus.
       * @param getter The function pointer to use for publishing data via const-reference.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Value>
      void publish( const Value& (*getter)(), Keys... args ) ;
      
      /** Method to set an indexed publisher in the bus.
       * @param getter The function pointer to use for publishing data via copy.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Value>
      void publish( Value (*getter)( unsigned ), Keys... args ) ;
      
      /** Method to set an indexed publisher in the bus.
       * @param getter The function pointer to use for publishing data via const-reference.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Value>
      void publish( const Value& (*getter)( unsigned ), Keys... args ) ;
      
      /** Method to set a publisher in the bus.
       * @param obj The object to use for calling the publishing function.
       * @param getter The function pointer to use for publishing data via copy.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void publish( Object* obj, Value (Object::*getter)(), Keys... args ) ;
      
      /** Method to set a publisher in the bus.
       * @param obj The object to use for calling the publishing function.
       * @param getter The function pointer to use for publishing data via const-reference.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void publish( Object* obj, const Value& (Object::*getter)(), Keys... args ) ;
      
      /** Method to set an indexed publisher in the bus.
       * @param obj The object to use for calling the publishing function.
       * @param getter The function pointer to use for publishing data via copy.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void publish( Object* obj, Value (Object::*getter)( unsigned ), Keys... args ) ;
      
      /** Method to set an indexed publisher in the bus.
       * @param obj The object to use for calling the publishing function.
       * @param getter The function pointer to use for publishing data via const-reference.
       * @param args The arguments that make up the name of the signal to send data over.
       */
      template<typename ... Keys, class Object, class Value>
      void publish( Object* obj, const Value& (Object::*getter)( unsigned ), Keys... args ) ;
      
      /** Method to recieve the channel this Bus is on.
       * @return unsigned The channel this bus is sending data on.
       */
      unsigned id() ;

      /** Method to set the channel this object uses for data transfer.
       * @note All objects subscribed to this channel can interact with others ONLY in the same channel.
       * @param id The channel to use for data transfer.
       */
      void setChannel( unsigned id ) ;
      
    private:
      
      /** Template class to encapsulate a publisher that emits via object.
       */
      template<class Object, class Type, bool Referenced, bool indexed>
      class MethodPublisher : public Publisher
      {
        public:
          typedef void ( Object::*Callback )( Type ) ;
          MethodPublisher( Object* obj, Callback callback ) ;
          
          virtual const void* publish( unsigned index = 0 ) ;
        private:
          Object*  object   ;
          Type     local    ;
          Callback callback ;
      };
      
      /** Template class to encapsulate a publisher that emits via function.
      */
      template<class Type, bool Referenced, bool Indexed>
      class FunctionPublisher : public Publisher
      {
        public:
          typedef void ( *Callback )( Type ) ;
          FunctionPublisher( Callback callback ) ;
          
          virtual const void* publish( unsigned index = 0 ) ;
        private:
          Type     local    ;
          Callback callback ;
      };
      
      /** Template class to encapsulate a subscriber that recieves data via object.
       */
      template<class Object, class Type, bool Referenced, bool indexed>
      class MethodSubscriber : public Subscriber
      {
        public:
          typedef void (Object::*Callback                  )( Type                  ) ;
          typedef void (Object::*IndexedCallback           )( unsigned, Type        ) ;
          typedef void (Object::*ReferencedCallback        )( const Type&           ) ;
          typedef void (Object::*IndexedReferencedCallback )( unsigned, const Type& ) ;
          
          MethodSubscriber( Object* obj, Callback callback ) ;
          
          void execute( const void* pointer, unsigned idx = 0 ) ;
        private:
          Object*  object   ;
          Callback callback ;
      };

      /** Template class to encapsulate a subscriber that recieves data via function.
       */      
      template<class Type, bool Referenced, bool indexed>
      class FunctionSubscriber : public Subscriber
      {
        public:
          typedef void (*Callback)( Type ) ;

          FunctionSubscriber( Callback callback ) ;
          
          void execute( const void* pointer, unsigned idx = 0 ) ;
        private:
          Callback callback ;
      };
      
      /** Forward declared structure to contain this object's internal data.
       */
      struct BusData* bus_data ;
      
      /** Method to retrieve a reference to this object's internal data.
       * @return Reference to this object's internal data.
       */
      BusData& data() ;
      
      /** Method to retrieve a const-reference to this object's internal data.
       * @return Cosnt-reference to this object's internal data.
       */
      const BusData& data() const ;
      
      /** Method to enroll a publisher in this bus.
       * @param key The key of signal to use to publish over.
       * @param publisher The publisher object to use for handling data.
       * @param type_id The hash representing the type of data being transferred.
       * @param type_name The name of the type being transferred.
       */
      void enrollBase( const Key& key, Publisher* publisher, unsigned type_id, const char* type_name ) ;
      
      /** Method to enroll a subscriber in this bus.
       * @param key The key of signal to use to subscribe to.
       * @param subscriber The subscriber object to use for handling data.
       * @param type_id The hash representing the type of data being transferred.
       * @param type_name The name of the type being transferred.
       */
      void enrollBase( const Key& key, Subscriber* subscriber, unsigned type_id, const char* type_name ) ;
      
      /** Method to manually emit data over the data bus.
       * @param key The key of signal to use to publish over.
       * @param value The value to send over the busu.
       * @param type_id The hash representing the type of data being transferred.
       * @param idx The index of data to send over.
       * @param type_name The name of the type being transferred.
       */
      void emitBase( const Key& key, const void* value, unsigned type_id, unsigned idx, const char* type_name ) ;
  };
  
  template<typename Type>
  const TypeInfo& typeinfo()
  {
    static TypeInfo type_info ;
    #if defined( __GNUC__ )
    const char* base_str  = __PRETTY_FUNCTION__ ;
    const char  beg_str[] = "[with Type ="      ;
    const char  end_str[] = "]"                 ;
    #elif defined( _MSC_VER )
    const char* base_str  = __FUNCSIG__ ;
    const char  beg_str[] = "typeinfo<" ;
    const char  end_str[] = ">("        ;    
    #elif defined( __clang__ )
    const char* base_str  = __PRETTY_FUNCTION__ ;
    const char  beg_str[] = "[Type = "          ;
    const char  end_str[] = "]"                 ;    
    #else
    const char* base_str  = "[UNKNOWN_TYPE]" ;
    const char  beg_str[] = "[" ;
    const char  end_str[] = "]" ;    
    #endif 
    
    type_info.ctti_name   = base_str + find( base_str, beg_str )                   ;
    type_info.ctti_length = find( base_str, end_str ) - find( base_str, beg_str )  ;
    type_info.ctti_hash   = hash( type_info.ctti_name, 0, type_info.ctti_length )  ;
    
    return type_info ;
  }
  
  template<typename T>
  Key concatenate( T type )
  {
    Key str ;
    
    str << type ;
    return str ;
  }

  template<typename T, typename... TYPES>
  Key concatenate( T first, TYPES... types )
  {
    Key str ;
    
    str << first ;
    str << concatenate( types... ) ;
    return str ;
  }

  constexpr unsigned hash( const char* str, unsigned start, unsigned end, unsigned h )
  {
    return start == end ? h : hash( str, start + 1, end, ( ( h << 5 ) + h ) + static_cast<unsigned>( str[ start ] ) ) ;
  }
  
  constexpr unsigned find( const char* str, const char* substr )
  {
    unsigned current = 0 ;
    
    for( unsigned match = 0; substr[ match ] != '\0' && str[ current ] != '\0'; ++current )
    {
      match = str[ current ] == substr[ match ] ? match + 1 : 0 ;
    }
    
    return current ;
  }
  
  template<class Type, bool Referenced, bool Indexed>
  Bus::FunctionSubscriber<Type, Referenced, Indexed>::FunctionSubscriber( FunctionSubscriber<Type, Referenced, Indexed>::Callback callback )
  {
    this->callback = callback ;
  }
  
  template<class Type, bool Referenced, bool Indexed>
  void Bus::FunctionSubscriber<Type, Referenced, Indexed>::execute( const void* pointer, unsigned idx )
  {
    
    typedef void ( *Callback                  )( Type                  ) ;
    typedef void ( *IndexedCallback           )( unsigned, Type        ) ;
    typedef void ( *ReferencedCallback        )( const Type&           ) ;
    typedef void ( *IndexedReferencedCallback )( unsigned, const Type& ) ;
    
    if( Referenced )
    {
      if( Indexed )
      {
        auto cb = reinterpret_cast<IndexedReferencedCallback>( this->callback ) ;
        ( cb )( idx, *static_cast<const Type*>( pointer ) ) ;
      }
      else
      {
        auto cb = reinterpret_cast<ReferencedCallback>( this->callback ) ;
        ( cb )( *static_cast<const Type*>( pointer ) ) ;
      }
    }
    else
    {
      if( Indexed )
      {
        auto cb = reinterpret_cast<IndexedCallback>( this->callback ) ;
        ( cb )( idx, *static_cast<const Type*>( pointer ) ) ;
      }
      else
      {
        auto cb = reinterpret_cast<Callback>( this->callback ) ;
        ( cb )( *static_cast<const Type*>( pointer ) ) ;
      }
    }
  }
  
  template<class Object, class Type, bool Referenced, bool Indexed>
  Bus::MethodSubscriber<Object, Type, Referenced, Indexed>::MethodSubscriber( Object* obj, Bus::MethodSubscriber<Object, Type, Referenced, Indexed>::Callback callback )
  {
    this->object   = obj      ;
    this->callback = callback ;
  }
  
  template<class Object, class Type, bool Referenced, bool Indexed>
  void Bus::MethodSubscriber<Object, Type, Referenced, Indexed>::execute( const void* pointer, unsigned idx )
  {
    typedef void ( Object::*Callback                  )( Type                  ) ;
    typedef void ( Object::*IndexedCallback           )( unsigned, Type        ) ;
    typedef void ( Object::*ReferencedCallback        )( const Type&           ) ;
    typedef void ( Object::*IndexedReferencedCallback )( unsigned, const Type& ) ;
    
    if constexpr( Referenced )
    {
      if constexpr( Indexed )
      {
        auto cb = reinterpret_cast<IndexedReferencedCallback>( this->callback ) ;
        ( ( this->object )->*( cb ) )( idx, *static_cast<const Type*>( pointer ) ) ;
      }
      else
      {
        auto cb = reinterpret_cast<ReferencedCallback>( this->callback ) ;
        ( ( this->object )->*( cb ) )( *static_cast<const Type*>( pointer ) ) ;
      }
    }
    else
    {
      if constexpr( Indexed )
      {
        auto cb = reinterpret_cast<IndexedCallback>( this->callback ) ;
        ( ( this->object )->*( cb ) )( idx, *static_cast<const Type*>( pointer ) ) ;
      }
      else
      {
        auto cb = reinterpret_cast<Callback>( this->callback ) ;
        ( ( this->object )->*( cb ) )( *static_cast<const Type*>( pointer ) ) ;
      }
    }
  }
  
  template<class Type, bool Referenced, bool indexed>
  Bus::FunctionPublisher<Type, Referenced, indexed>::FunctionPublisher( FunctionPublisher::Callback callback )
  {
    this->callback = callback ; 
  }
  
  template<class Type, bool Referenced, bool Indexed>
  const void* Bus::FunctionPublisher<Type, Referenced, Indexed>::publish( unsigned idx )
  {
    
    typedef Type        (*Callback                  )(          ) ;
    typedef Type        (*IndexedCallback           )( unsigned ) ;
    typedef const Type& (*ReferencedCallback        )(          ) ;
    typedef const Type& (*IndexedReferencedCallback )( unsigned ) ;
    
    if( Referenced )
    {
      if( Indexed )
      {
        auto cb = reinterpret_cast<IndexedReferencedCallback>( this->callback ) ;
        return static_cast<const void*>( &( cb )( idx ) ) ;
      }
      else
      {
        auto cb = reinterpret_cast<ReferencedCallback>( this->callback ) ;
        return static_cast<const void*>( &( cb )() ) ;
      }
    }
    else
    {
      if( Indexed )
      {
        auto cb = reinterpret_cast<IndexedCallback>( this->callback ) ;
        this->local = ( cb )( idx ) ;
        
        return static_cast<const void*>( &this->local ) ;
      }
      else
      {
        auto cb = reinterpret_cast<Callback>( this->callback ) ;
        this->local = ( cb )() ;
        
        return static_cast<const void*>( &this->local ) ;
      }
    }
  }
  
  template<class Object, class Type, bool Referenced, bool Indexed>
  Bus::MethodPublisher<Object, Type, Referenced, Indexed>::MethodPublisher( Object* obj, Bus::MethodPublisher<Object, Type, Referenced, Indexed>::Callback callback )
  {
    this->object   = obj      ;
    this->callback = callback ; 
  }
  
  template<class Object, class Type, bool Referenced, bool Indexed>
  const void* Bus::MethodPublisher<Object, Type, Referenced, Indexed>::publish( unsigned idx )
  {
    /** Note: Is this better to do than to just make different abstract classes for each specification? Surely the compiler figures this stuff out right? */
    typedef Type        (Object::*Callback                  )(          ) ;
    typedef Type        (Object::*IndexedCallback           )( unsigned ) ;
    typedef const Type& (Object::*ReferencedCallback        )(          ) ;
    typedef const Type& (Object::*IndexedReferencedCallback )( unsigned ) ;
    
    if( Referenced )
    {
      if( Indexed )
      {
        auto cb = reinterpret_cast<IndexedReferencedCallback>( this->callback ) ;
        return static_cast<const void*>( &( ( ( this->object )->*( cb ) )( idx ) ) ) ;
      }
      else
      {
        auto cb = reinterpret_cast<ReferencedCallback>( this->callback ) ;
        return static_cast<const void*>( &( ( ( this->object )->*( cb ) )() ) ) ;
      }
    }
    else
    {
      if( Indexed )
      {
        auto cb = reinterpret_cast<IndexedCallback>( this->callback ) ;
        this->local = ( ( ( this->object )->*( cb ) )( idx ) ) ;
        
        return static_cast<const void*>( &this->local ) ;
      }
      else
      {
        auto cb = reinterpret_cast<Callback>( this->callback ) ;
        this->local = ( ( ( this->object )->*( cb ) )() ) ;
        
        return static_cast<const void*>( &this->local ) ;
      }
    }
  }
  
  template<class Value, typename ... Keys>
  void Bus::emitIndexed( const Value& value, unsigned idx, Keys... args )
  {
    const TypeInfo ctti = typeinfo<Value>() ;
    Key key ;
    
    key = ::karma::concatenate( "", args... ) ;
    this->emitBase( key, static_cast<const void*>( &value ), ctti.ctti_hash, idx, ctti.ctti_name ) ;
  }
  
  template<class Value, typename ... Keys>
  void Bus::emit( const Value& value, Keys... args )
  {
    const TypeInfo ctti = typeinfo<Value>() ;
    Key key ;
    
    key = ::karma::concatenate( "", args... ) ;
    this->emitBase( key, static_cast<const void*>( &value ), ctti.ctti_hash, 0, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::enroll( void (*setter)( Value ), Keys... args )
  {
    typedef Bus::FunctionSubscriber<Value, false, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback* callback ;
    Key       key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, dynamic_cast<Bus::Subscriber*>( callback ), ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::enroll( void (*setter)( const Value& ), Keys... args )
  {
    typedef Bus::FunctionSubscriber<Value, true, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback* callback ;
    Key       key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::enroll( void (*setter)( unsigned, Value ), Keys... args )
  {
    typedef Bus::FunctionSubscriber<Value, false, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback callback ;
    Key      key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )          ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::enroll( void (*setter)( unsigned, Value const & ), Keys... args )
  {
    typedef Bus::FunctionSubscriber<Value, true, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback callback ;
    Key      key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Object, class Value>
  void Bus::enroll( Object* obj, void (Object::*setter)( Value ), Keys... args )
  {
    typedef Bus::MethodSubscriber<Object, Value, false, false> Callback ;  
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Object, class Value>
  void Bus::enroll( Object* obj, void (Object::*setter)( Value const & ), Keys... args )
  {
    typedef Bus::MethodSubscriber<Object, Value, true, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Object, class Value>
  void Bus::enroll( Object* obj, void (Object::*setter)( unsigned, Value ), Keys... args )
  {
    typedef Bus::MethodSubscriber<Object, Value, false, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Object, class Value>
  void Bus::enroll( Object* obj, void (Object::*setter)( unsigned, Value const & ), Keys... args )
  {
    typedef Bus::MethodSubscriber<Object, Value, true, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( setter ) ) ;
    key      = ::karma::concatenate( "", args... )                    ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::publish( Value (*getter)(), Keys... args )
  {
    typedef Bus::FunctionPublisher<Value, false, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback* callback ;
    Key       key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::publish( const Value& (*getter)(), Keys... args )
  {
    typedef Bus::FunctionPublisher<Value, true, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback* callback ;
    Key       key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::publish( Value (*getter)( unsigned ), Keys... args )
  {
    typedef Bus::FunctionPublisher<Value, false, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Value>
  void Bus::publish( const Value& (*getter)( unsigned ), Keys... args )
  {
    typedef Bus::FunctionPublisher<Value, true, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )               ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Object, class Value>
  void Bus::publish( Object* obj, Value (Object::*getter)(), Keys... args )
  {
    typedef Bus::MethodPublisher<Object, Value, false, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )                    ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
  
  template<typename ... Keys, class Object, class Value>
  void Bus::publish( Object* obj, const Value& (Object::*getter)(), Keys... args )
  {
    typedef Bus::MethodPublisher<Object, Value, true, false> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )                    ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }

  template<typename ... Keys, class Object, class Value>
  void Bus::publish( Object* obj, Value (Object::*getter)( unsigned ), Keys... args )
  {
    typedef Bus::MethodPublisher<Object, Value, false, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )                    ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }

  template<typename ... Keys, class Object, class Value>
  void Bus::publish( Object* obj, const Value& (Object::*getter)( unsigned ), Keys... args )
  {
    typedef Bus::MethodPublisher<Object, Value, true, true> Callback ;
    const TypeInfo ctti = typeinfo<Value>() ;
    Callback *callback ;
    Key       key      ;
    
    callback = new Callback( obj, reinterpret_cast<typename Callback::Callback>( getter ) ) ;
    key      = ::karma::concatenate( "", args... )                    ;
    
    this->enrollBase( key, callback, ctti.ctti_hash, ctti.ctti_name ) ;
  }
}

#endif

