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

#ifndef IRIS_IO_OBJECT_LOADER_H
#define IRIS_IO_OBJECT_LOADER_H

namespace iris
{
  namespace io
  {
    /** A Class to manage symbols loaded from a Shared Object.
     */
    template<class RETURN, class ... ARGS>
    class Symbol
    {
      public:
        
        /** Default Constructor.
         */
        Symbol() ;
        
        /** Default Deconstructor.
         */
        ~Symbol() = default ;
        
        /** Conversion operator to check for validity of this object.
         * @return Whether this object is valid or not.
         */
        operator bool() const ;

        /** Paren operator to call this symbol as a function.
         * @param args The arguments specified in the template parameters.
         * @return The return type that this object's template parameters specify.
         */
        RETURN operator()( ARGS ... args ) ;
        
        /** Assignment operator.
         * @param func The function pointer to associate with this object.
         * @return This object after the assignment operation.
         */
        Symbol& operator=( void (*func)() ) ;

      private:
        RETURN (*func)( ARGS ... args ) ;
    };

    /** A Class to manage loading of shared objects from a .DLL / .SO.
     */
    class ObjectLoader
    {
      public: 
        typedef void (*DL_FUNC)() ;
        
        /** Default Constructor.
         */
        ObjectLoader() ;
        
        /** Default Deconstructor. Releases this object's allocated data.
         */
        ~ObjectLoader() ;
        
        /** Method to load the shared object at the specified path.
         * @param lib_path The path to use for loading the shared object.
         */
        void load( const char* lib_path ) ;
        
        /** Method to reset this object and release all allocated data and opened shared objects.
         */
        void reset() ;

        /** Method to return a function pointer to a symbol loaded by a Shared Object.
         * @param symbol_name The name of symbol to attempt to load.
         * @return The function pointer associated with the loaded symbol. Returns nullptr if the symbol is not valid.
         */
        DL_FUNC symbol( const char* symbol_name ) ;
      private:
        
        /** The forward declared structure containing this object's data.
         */
        struct ObjectLoaderData *loader_data ;
        
        /** Method to retrieve a reference to this object's internal data structure.
         * @return A reference to this object's internal data structure.
         */
        ObjectLoaderData& data() ;
        
        /** Method to retrieve a reference to this object's internal data structure.
         * @return A reference to this object's internal data structure.
         */
        const ObjectLoaderData& data() const ;
    };
    
    template<class RETURN, class ... ARGS>
    Symbol<RETURN, ARGS... >::Symbol()
    {
      this->func = nullptr ;
    }
    
    template<class RETURN, class ... ARGS>
    Symbol<RETURN, ARGS... >::operator bool() const
    {
      if( this->func == nullptr ) return false ;
      return true ;
    }
    
    template<class RETURN, class ... ARGS>
    RETURN Symbol<RETURN, ARGS... >::operator()( ARGS ... args )
    {
      return ( this->func )( args... ) ;
    }
    
    template<class RETURN, class ... ARGS>
    Symbol<RETURN, ARGS...>& Symbol<RETURN, ARGS...>::operator=( void (*func)() )
    {
      this->func = reinterpret_cast<RETURN(*) (ARGS ... )>( func ) ;
      return *this ;
    }
  }
}

#endif
