#include "Pool.h"
#include "PoolData.h"
#include <map>
#include <string>

namespace data
{
  namespace module
  {
    typedef std::map<std::string, CData> DataMap ;
    
    static DataMap pool ;

    Pool::Pool()
    {
      
    }

    Pool::~Pool()
    {

    }
    
    CData& Pool::operator[]( const char* key )
    {
      return pool[ std::string( key ) ] ;
    }


  }
}
