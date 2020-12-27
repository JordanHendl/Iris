
#ifndef KARMA_LOADER_H
#define KARMA_LOADER_H

namespace karma
{
  class Module ;
  
  class Descriptor
  {
    public:
      Descriptor() ;
      ~Descriptor() ;
      Descriptor( const Descriptor& desc ) ;
      void operator=( const Descriptor& desc ) ;
      Module* create( unsigned version = 0 ) ;
      void destroy( Module* module, unsigned version = 0 ) ;
    private:
      
      friend class LoaderData ;
      void initalize( const char* module_path ) ;

      struct DescriptorData* desc_data ;
      DescriptorData& data() ;
      const DescriptorData& data() const ;
  };

  class Loader
  {
    public:
      Loader() ;
      ~Loader() ;
      Loader( const Loader& loader ) ;
      void operator=( const Loader& loader ) ;
      void initialize( const char* module_path ) ;
      const Descriptor& descriptor( const char* module_type ) const ;
      bool hasDescriptor( const char* module_type ) const ;
      void reset() ;
    private:
      struct LoaderData *loader_data ;
      LoaderData& data() ;
      const LoaderData& data() const ;
  };
}

#endif /* LOADER_H */

