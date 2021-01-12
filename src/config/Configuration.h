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
#ifndef KARMA_JSON_CONFIGURATION_H
#define KARMA_JSON_CONFIGURATION_H

namespace karma
{
  namespace config
  {
    namespace json
    {
      /** Forward Declared object from Parser.h.
       */
      class Token ;
    }

    /** Class to configure the rest of the program using a JSON file.
     * See @src/data/Bus.h/cpp for how data is sent and recieved.
     */
    class Configuration
    {
      public:
      
        /** Default Constructor. Initializes this object's data.
         */
        Configuration() ;

        /** Default Deconstructor. Releases this object's allocated data.
         */
        ~Configuration() ;
        
        /** Method to retrieve whether or not this object is initialized.
         * @return Whether or not this object is initialized.
         */
        bool isInitialized() const ;

        /** Method to retrieve a const reference to this object's beginning of the parsed map.
         * @note This object automatically pulses data through it's internal data bus. This is for manual iteration.
         * @return const-reference to the beginning of this object's parsed map.
         */
        const json::Token& begin() const ;

        /** Method to retrieve a const reference to this object's end of the parsed map.
         * @note This object automatically pulses data through it's internal data bus. This is for manual iteration.
         * @return const-reference to the end of this object's parsed map.
         */
        const json::Token& end() const ;

        /** Method to initialize this object with the input JSON file path.
         * @param path The path to the JSON file to process on the file system.
         * @param channel The channel of data bus to pulse information to.
         */
        void initialize( const char* path, unsigned channel = 0 ) ;

      private:

        /** Forward declared structure to contain this object's interal data.
         */
        struct ConfigurationData *config_data ;

        /** Method to retrieve a reference to this object's internal data.
         * @return A reference to this object's internal data.
         */
        ConfigurationData& data() ;

        /** Method to retrieve a const-reference to this object's internal data.
         * @return A const-reference to this object's internal data.
         */
        const ConfigurationData& data() const ;
    };
  }
}
#endif