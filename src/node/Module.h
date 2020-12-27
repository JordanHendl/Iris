#ifndef KARMA_MODULE
#define KARMA_MODULE

// Defines for different OS Shared Library Exports.
#ifdef _WIN32
  #include <windows.h>
  #define exported_function extern "C" __declspec( dllexport )
#else 
  #define exported_function extern "C"
#endif

namespace karma
{
  /** Class for describing a GPU Vulkan Render/Compute Pass.
   */
  class Module
  { 
    public:
      /** Default Constructor. Initializes this object's data.
       */
      Module() ;
      
      /** Virtual deconstructor. Needed for inheritance.
       */
      virtual ~Module() ;
      
      /** Method to initialize this module after being configured.
       */
      virtual void initialize() = 0 ;
      
      /**
       * @param bus_id The channel of bus to subscribe inputs on.
       * @param gpu The gpu to use for all GPU operations.
       */
      virtual void subscribe( const char* pipeline, unsigned id ) ;

      /** Method to shut down this object's operation.
       */
      virtual void shutdown() = 0 ;
      
      /** Method to execute a single instance of this module's operation.
       */
      virtual void execute() = 0 ;
      
      /**  Method to retrieve the id of module in this graph.
       * @return The id of module in this graph.
       */
      unsigned id() const ;
      
      /** Method to set the id of this module.
       * @param id The id to associate with this module.
       */
      void setId( unsigned id ) ;

      /** Method to start operations of this module.
       * @return 
       */
      void start() ;
      
      /** Method to stop operation of this module.
       * @return Whether the module is stopped or not.
       */
      bool stop() ;

      /** Method to set the version of this module.
       * @param version The version of this module.
       */
      void setVersion( unsigned version ) ;

      /** Method to set the name of this module.
       * @param name
       */
      void setName( const char* name ) ;
      
      /** Method to set the type name of this module.
       * @param type_name The name of the type of module this is.
       */
      void setTypeName( const char* name ) ;
      
      /** Method to retrieve the version of this module.
       * @return The version of this module
       */
      unsigned version() const ;
      
      /**
       * @return 
       */
      const char* type() const ;

      /** Method to retrieve the name of this module.
       * @return The name of this module.
       */
      const char* name() const ;
      
    private:
      
      /**
       */
      struct ModuleData* module_data ;
      
      /** Method to retrieve a reference to this object's internal data structure.
       * @return Reference to this object's internal data structure.
       */
      ModuleData& data() ;

      /** Method to retrieve a const-reference to this object's internal data structure.
       * @return Const-reference to this object's internal data structure.
       */
      const ModuleData& data() const ;
  };
}
#endif