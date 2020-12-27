#ifndef DATA_POOL_INFO_H
#define DATA_POOL_INFO_H

namespace data
{
  namespace module
  {
    class DataType
    {
      public:
        DataType() ;
        bool operator<( DataType type ) const ;

      private:
        typedef DataType (*Identifier)() ;
        Identifier ident ;
      
        DataType( Identifier id ) ;
      
        template<class T>
        friend DataType dataType() ;
     } ;
 
     class CData
     {
       public:
         CData() ;
         ~CData() ;
       
         template<class T>
         void set( T value ) ;

         template<class T>
         T get() ;

         template<class T>
         void reset() ;

       private:
           void* getBase( DataType type ) ;
          void setBase( DataType type, void* data ) ;
          void resetBase( DataType type ) ;

          struct DataContainer* data_cont ;
          DataContainer& data() ;
          const DataContainer& data() const ;

          friend DataContainer ;
     };

    template<class T>
    DataType dataType() ;

    template<class T>
    void CData::set( T value )
    {
      T* temp = new T( value ) ;
      setBase( dataType<T>(), reinterpret_cast<void*>( temp ) ) ;
    }

    template<class T>
    T CData::get()
    {
      return *reinterpret_cast<T*>( getBase( dataType<T>() ) ) ;
    }
  
    template<class T>
    void CData::reset()
    {
      delete( reinterpret_cast<T*>( getBase( dataType<T>() ) ) ) ;
      resetBase( dataType<T>() ) ;
    }
  
     template<class T>
     DataType dataType()
     {
       return DataType( &dataType<T> ) ;
     }
  }
}

 
#endif
