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

#ifndef IRIS_INTERFACE_H
#define IRIS_INTERFACE_H

/** Object to interface and interact with the Iris Library.
 */
class Iris
{
  public:
    
    /** Default Constructor.
     */
    Iris() ;
    
    /** Deconstructor.
     */
    ~Iris() ;
    
    /** Method to initialize the Iris Library.
     * @param setup_json_path The path on the filesystem to the JSON file describing this iris execution.
     */
    void initialize( const char* setup_json_path ) ;
    
    /** Method to retrieve whether or not the iris runtime is currently running.
     * @return Whether or not Iris is currently running.
     */
    bool running() const ;

    /** Method to shutdown the Iris Library's operations & free all allocated data..
     */
    void shutdown() ;
    
  private:
    
    /** Forward Declared structure to contain this object's data.
     */
    struct IrisData* iris_data ;
    
    /** Method to retrieve a reference to this object's internal data structure.
     * @return Reference to this object's internal data structure.
     */
    IrisData& data() ;
    
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
    const IrisData& data() const ;
};

#endif

