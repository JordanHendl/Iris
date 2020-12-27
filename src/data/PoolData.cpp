#include "PoolData.h"
#include <map>

namespace data
{
  namespace module
  {
    struct DataContainer
    {
       typedef std::multimap<DataType, void*> DataMap ;
  
       DataMap map ;
    };

    DataType::DataType()
    {
      this->ident = NULL ;
    }

    DataType::DataType( Identifier id )
    {
      this->ident = id ;
    }
    
    bool DataType::operator <( DataType type ) const
    {
      return this->ident < type.ident ;
    }
    
    CData::CData()
    {
      this->data_cont = new DataContainer() ;
    }
    
    CData::~CData()
    {
      data().map.clear() ;
    }

    DataContainer& CData::data()
    {
      return *this->data_cont ;
    }

    const DataContainer& CData::data() const
    {
      return *this->data_cont ;
    }

    void CData::setBase( DataType type, void* d )
    {
      data().map.insert( { type, d } ) ;
    }

    void* CData::getBase( DataType type )
    {
      if( data().map.find( type ) != data().map.end() )
      {
        return data().map.find( type )->second ;
      }
      else
      {
        return data().map.begin()->second ;
      }
      
    }

    void CData::resetBase( DataType type )
    {
      data().map.erase( type ) ;
    }
  }
}