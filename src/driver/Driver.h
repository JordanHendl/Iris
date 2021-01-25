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

/* 
 * File:   IrisDriver.h
 * Author: Jordan Hendl
 *
 * Created on January 3, 2021, 5:37 PM
 */

#ifndef IRIS_DRIVER_H
#define IRIS_DRIVER_H

/** Object for running chauffeur application.
 */
class IrisDriver
{
  public:
    
    /** Default constructor.
     */
    IrisDriver() ;
    
    /** Deconstructor.
     */
    ~IrisDriver() ;
    
    /** Method to initialize the Chauffuer program with arguments.
     * @param arguments The string arguments to parse and use for runtime.
     */
    void initialize( unsigned argc, const char* arguments[] ) ;
    
    /** Method to run the program.
     * @return The error code to return.
     */
    int run() ;
};

#endif /* CHAUFFEUR_H */

