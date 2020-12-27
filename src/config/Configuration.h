/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Configuration.h
 * Version      : 1.0
 * Date created : 6/21/2020
 * Purpose      : Declaration for Configuration object to configure an entire program pipeline.
**********************************************************************/
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
    };

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