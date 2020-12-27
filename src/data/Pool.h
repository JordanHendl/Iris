#ifndef DATA_MODULE_INFO_POOL_H
#define DATA_MODULE_INFO_POOL_H

namespace data
{
  namespace module
  {
    class CData ;
    class Pool
    {
      public:
        Pool() ;
        CData& operator[]( const char* key ) ; 
        ~Pool() ;
      private:
    };
  }
}
#endif